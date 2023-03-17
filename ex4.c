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


return 0;
}
