#include <stdio.h>              /* I/O lib         ISOC     */
#include <stdlib.h>             /* Standard Lib    ISOC     */
#include "cblas.h"              /* Basic Linear Algebra I/O */

int main(int argc, char **argv) {
  double a[4*2] = {  1, 2,   /* CblasRowMajor */
                     6, 7, 
                    11,12,
                    16,17,
                  };
  double b[2*3] = {  1,0,1,   /* CblasRowMajor */
                     1,0,1
                    };
  double c[4*3];

  double d[4*4] = {  1, 2, 3, 4,  /* CblasRowMajor */
                     6, 7, 8, 9,
                    11,12,13,14,
                    16,17,18,19
                  };
  double e[4*4] = {  1, 0, 0, 0,  /* CblasRowMajor */
                     0, 0, 1, 0,
                     0, 1, 0, 0,
                     0, 0, 0, 1
                  };

            /* row_order      transform     transform     rowsA colsB K  alpha  a  lda  b  ldb beta c   ldc */
   cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, 4,    3,    2, 1.0,   a,   2, b, 3,  0.0, c,  3);

for (int i=0;i<12;i++) {
printf("%f\n", c[i]);
}

   return 0;
} /* end func main */
