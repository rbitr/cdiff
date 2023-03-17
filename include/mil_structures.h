#pragma once

#include <stdio.h>
#include <stdlib.h>

#define MAX_DIMS 2

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
