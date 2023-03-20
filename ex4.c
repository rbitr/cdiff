// Tests for tensors (currently max 2d)

#include <stdio.h>
#include <stdlib.h>

#include "include/mil_structures.h"

int main() {

mil_tensor * t;

size_t s[2] = {6,-1}; // all unused dimensions should be -1;;

t = mil_zeros(s);

char * o = mil_tensor_tostring(t);
printf("%lu\n", mil_flat_len(t->shape));
printf("%s\n", o);

free(o);

free(t);

t = mil_ones(s);

o = mil_tensor_tostring(t);

printf("%s\n", o);

free(o);
free(t);

t = mil_init(s,34.2);

o = mil_tensor_tostring(t);

printf("%s\n", o);

free(o);
free(t);

size_t s2[] = {4,5};
size_t scalar[] = {1, -1};
t = mil_zeros(s2);
mil_tensor * t2 = malloc(sizeof(mil_tensor));

t2 = mil_ones(scalar);

mil_tensor * t3 = malloc(sizeof(mil_tensor));

t3 = mil_zeros(s2);

mil_add(t, t2, t3);
mil_add(t2, t3, t);

o = mil_tensor_tostring(t);

printf("%s\n", o);

free(o);
free(t);
free(t2);
free(t3);

return 0;
}
