#include <math.h>               /* Math stuff      ISOC  */
#include <stdio.h>              /* I/O lib         ISOC  */
#include <stdlib.h>             /* Standard Lib    ISOC  */
#include "cblas.h"           /* Basic Linear Algebra I/O */

int main(int argc, char **argv) {
/*  double a[4*5] = {  1, 2, 3, 4, 5,
                     6, 7, 8, 9,10,
                    11,12,13,14,15,
                    16,17,18,19,20
                  };
  double x[5]  = {2,3,4,5,6};
  double y[4];
*/

  double a[4*2] = {  1, 2, 
                     6, 7, 
                    11,12,
                    16,17,
                  };
  double x[2]  = {2,3};
  double y[4];

           /* row_order      transform     lenY lenX alpha  a  lda  X  incX  beta  Y, incY */
  cblas_dgemv(CblasRowMajor, CblasNoTrans, 4,   2,   1,     a,   2, x, 1,    0,    y, 1);

   for (int k=0;k<4;k++) {
printf("%f\n", y[k]);
} 
   
//  cblas_dger(CblasRowMajor,  4,   5,   1,     y, 1,    x, 1,   a, 5);

  return 0;
} /* end func main */
