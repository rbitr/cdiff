#include <stdio.h>

#define COPY(x, y, l) { \
for (int id=0;id<l;id++) { \
  y[id]=x[id];}}

#define PRINT(x,l,s) {\
for (int id=0;id<l;id++) {\
printf (s,x[id]);\
printf ("\n");}}\

int main () {

double x [] = {1,2,4,5};
double y[4];

COPY(x,y,4)
PRINT(y,4,"%f ")

return 0;

}
