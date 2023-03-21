#include <stdio.h>
#include <stdlib.h>

#include "../include/mil_structures.h"
#include "../include/other/ht.h"
#include "../include/other/ht.c"

int main () {

	ht * weights = ht_create();

	mil_tensor * t1 = malloc(sizeof(mil_tensor));
	mil_tensor * t2 = malloc(sizeof(mil_tensor));

	double a[] = {1, 3, 3, 5, 4, 4};
	double b[] = {1, 1, 1, 2, 2, 2, 3, 3, 3};

	size_t sa[] = {2,3};
	size_t sb[] = {3,3}; 

	t1->data = a;
	A_COPY(sa,t1->shape,2);

	t2->data = b;
	A_COPY(sb,t2->shape,2);
	
	ht_set(weights, "W1", t1);
	ht_set(weights, "W2", t2);

	mil_tensor *x = ht_get(weights, "W1");

	char *o = mil_tensor_tostring(x);

	printf("%s\n", o);

	free(o);
	o = mil_tensor_tostring(ht_get(weights, "W2"));
	printf("%s\n", o);
	

	return 0;
}
