#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#define TRUE 1
#define FALSE !TRUE
#define MAX_DIMS 2

#define A_COPY(a,b,l) do{ for (int i=0;i<(l);i++) b[i]=a[i]; } while (FALSE)

size_t mil_flat_len(size_t * shape) {
        size_t tlen = 1;
        int ix = 0;

        while ( ix<MAX_DIMS && shape[ix] != -1 ) {
                tlen = tlen * shape[ix];
                ix++;
        }
	
	return tlen;
}

void mil_size_t_array_copy(size_t *x,size_t *y,size_t l) { // copy 
for (int i=0;i<l;i++) y[i] =x[i];
}

void mil_fill(double *x, double v, size_t l) {
for (int i=0;i<l;i++) x[i]=v;
}

typedef struct mil_tensor mil_tensor;

struct mil_tensor {

	size_t shape[MAX_DIMS];
	double * data;

};

mil_tensor * mil_init(size_t * shape, double v) {

	size_t tlen = mil_flat_len(shape);
	//int ix = 0;
	//
	//while ( ix<MAXDIMS && shape[ix] != -1 ) {
	//	tlen = tlen * shape[ix];
	//	ix++;
	//}

	mil_tensor * r = malloc(sizeof(mil_tensor));
	mil_size_t_array_copy(shape,r->shape,MAX_DIMS);
	r->data = malloc(tlen*sizeof(double));

	mil_fill(r->data,v,tlen);

	return r;	
	

}

mil_tensor * mil_zeros(size_t * shape) {
	return mil_init(shape, 0.0);
}

mil_tensor * mil_ones(size_t * shape) {
	return mil_init(shape, 1.0);
}

int mil_is_scalar(mil_tensor *a) {
	return a->shape[0]==1 && a->shape[1] == -1;
}

int mil_are_equal_shape(mil_tensor *a, mil_tensor *b) {
	for (int i=MAX_DIMS-1;i>=0;i--) {
		if (a->shape[i] != b->shape[i]) {
			return FALSE;
		}
	}
	return TRUE;
}

int mil_last_dim(mil_tensor *a) {

	int ix = 0;
	while(a->shape[ix] != -1 && ix < MAX_DIMS) {
		ix++;
	}
	return ix-1;	

}

int mil_broadcast_compatible(mil_tensor *a, mil_tensor *b) {
	int ad = mil_last_dim(a);
	int bd = mil_last_dim(b);
	
	// for now, check if b is a row vector with same # columns as a
	if (a->shape[ad] == b->shape[bd]) {
		bd--;
		for (;bd>=0;bd--) {
			if (b->shape[bd] != 1) {
				return FALSE;
			}
		}
		return TRUE;
	}
	return FALSE;
	/*
	// Either dimensions are the wsame, or they are 1
	if (a->shape[ad] == b->shape[bd]) {
		for (;bd>=0;bd--) {
			ad--;
			if (ad < 0) return true;
			if (a->shape[ad] == b->shape[bd] ||
			    a->shape[ad] == 1 ||
			    b->shape[bd] == 1) {
				continue;
			}
			else {
				return false;
			}
		}
		return true;
	}*/	
}

void mil_add_const(mil_tensor *a, double c, mil_tensor * result) {
	size_t l = mil_flat_len(a->shape);
	// this should already be true, otherwise there are bigger problems
	// could be an assert
	for (int k=0;k<MAX_DIMS;k++) { assert (a->shape[k]==result->shape[k]); }

	for (int k=0;k<l;k++) {
		result->data[k] = a->data[k]+c;
	}	
} 

void mil_add_like(mil_tensor *a, mil_tensor *b, mil_tensor *result) {
        size_t l = mil_flat_len(a->shape);
        
	for (int k=0;k<MAX_DIMS;k++) { 
		assert (a->shape[k]==result->shape[k]); 
		assert (a->shape[k]==b->shape[k]); 
	}

        for (int k=0;k<l;k++) {
                result->data[k] = a->data[k]+b->data[k];
        }
}

void mil_broadcast_add(mil_tensor *a, mil_tensor *b, mil_tensor *result) {
	// start assuming the last dimension of b is the same as 
	// that of a, and the rest are 1
		
	for (int k=0;k<MAX_DIMS;k++) { assert (a->shape[k]==result->shape[k]); }
	int bl = b->shape[mil_last_dim(b)];
	assert (bl == mil_flat_len(b->shape));
	int al = mil_flat_len(a->shape);

	for (int ix=0;ix<al;ix++) {
		result->data[ix] = a->data[ix] + b->data[ix%bl];
	}
}

void mil_add(mil_tensor * a, mil_tensor *b, mil_tensor *result) {
// add a and b together with broadcast rules
	if (mil_is_scalar(a)) {
		mil_add_const(b,a->data[0], result);
		return;
	}
	else if (mil_is_scalar(b)) {
		mil_add_const(a,b->data[0], result);
		return;
	}
	else if (mil_are_equal_shape(a,b)) {
		mil_add_like(a,b,result);
		return;
	}
	
	else if (mil_broadcast_compatible(a,b)) {
		mil_broadcast_add(a,b,result);
		return;
	}
	else {
		fprintf(stderr, "addition not implemented for shapes\n");
		exit(-1);
	}
}




char * mil_tensor_tostring(mil_tensor * t) {
	int max_print_chars = 9;
	size_t l = mil_flat_len(t->shape);
	char * buf = malloc(l*max_print_chars*sizeof(char)); // max 7 characters to print each
	size_t b=0;
	// loop through 
	//b += sprintf(buf+b, "printing something\n");
	//b += sprintf(buf+b, "Tensor length: %lu\n", l);
	for (int i=0;i<l;i++) {
		
		b += sprintf(buf+b, "%.3f ", t->data[i]);
		if ((i+1) % t->shape[1] == 0) b+= sprintf(buf+b, "\n");

		if (b >= max_print_chars*(l-1)) return buf; // not foolproof 	

	}

	return buf;

}
