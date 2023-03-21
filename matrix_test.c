// Tests for tensors (currently max 2d)

#include <stdio.h>
#include <stdlib.h>

#include "include/mil_structures.h"

void print_with_line_after(char *s) {
	printf("%s\n-----------------------------------\n", s);
}

void test_matrix_op( void (* func)(mil_tensor *a, mil_tensor *b, mil_tensor *r),
			double *x, double *y, 
			size_t xdims [],
			size_t ydims [],
			size_t rdims []) {	
	mil_tensor *t = malloc(sizeof(mil_tensor));
	mil_tensor *u = malloc(sizeof(mil_tensor));
	mil_tensor *r = malloc(sizeof(mil_tensor));	
	t->data = malloc(mil_flat_len(xdims)*sizeof(double));
	u->data = malloc(mil_flat_len(ydims)*sizeof(double));
	r->data = malloc(mil_flat_len(rdims)*sizeof(double));
	A_COPY(xdims,t->shape,MAX_DIMS);
	A_COPY(x,t->data,mil_flat_len(xdims));
	A_COPY(ydims,u->shape,MAX_DIMS);
	A_COPY(y,u->data,mil_flat_len(ydims));
	A_COPY(rdims,r->shape,MAX_DIMS);

	func(t,u,r);

	char * ol = mil_tensor_tostring(t);
	char * or = mil_tensor_tostring(u);

	char *oo = mil_tensor_tostring(r);	

	printf("Left input\n%s\nRight input\n%s\nOutput\n%s\n", ol,or,oo);

	free(ol);
	free(or);
	free(oo);	

	free(t->data);
	free(u->data);
	free(r->data);
	free(t);
	free(u);
	free(r);

}


int main() {

	double x[] = {1, 2, 3,
			4, 5, 6,
			7, 8, 9,
			10, 11, 12};
	size_t xdims[] = {4, 3};
	
	double y[] = {1, 0, 0,
			0, 1, 0,
			0, 1, 0,
			0, 0, 1};
	size_t ydims[] = {4,3};

	double z[] = {5, 6, 7};
	size_t zdims[] = {3, -1};
	size_t alt_zdims[] = {1,3};

	double v[] = {3.3};
	size_t vdims[] = {1,-1};

	double m[] = {1,0, 0, 0,
			0, 1, 0, 0,
			0, 0, 1, 0};
	size_t mdims[] = {3,4};

	size_t rdims[] = {4,4};

	print_with_line_after("Adding like shapes");
	test_matrix_op(mil_add, x, y, xdims, ydims, xdims);	
	
	print_with_line_after("Broadcast addition");
	test_matrix_op(mil_add, x, z, xdims, alt_zdims, xdims);

	print_with_line_after("Matrix multiplication");
	test_matrix_op(mil_matmul, x, m, xdims, mdims, rdims);

	return 0;

}
