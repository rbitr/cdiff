// basic matrix multiplication routines using blas
#include <stdio.h>
#include "cblas.h"

void matrix_vector_mul(double *A, int rowsa, int colsa, 
		       double *x, int lenx, 
		       double *y) {

cblas_dgemv(CblasRowMajor, CblasNoTrans, rowsa,   colsa,   1,     A,   lenx, x, 1,    0,    y, 1);

}

void matrix_matrix_mul(double *A, int rowsa, int colsa,
			double *B, int rowsb, int colsb,
			double *C) {
 /* row_order      transform     transform     rowsA colsB K  alpha  a  lda  b  ldb beta c   ldc */
cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, rowsa,    colsb,    colsa, 1.0,   A,   colsa, B, colsb,  0.0, C,  rowsa);

}



int main(int argc, char **argv) {
  double a[4*5] = {  1, 2, 3, 4, 5,
                     6, 7, 8, 9,10,
                    11,12,13,14,15,
                    16,17,18,19,20
                  };
  double x[5]  = {2,3,4,5,6};
  double y[4];

  double b[5*4] = {  1, 0, 0, 0,   /* CblasRowMajor */
                     0, 0, 1, 0,
                     0, 1, 0, 0,
                     0, 0, 0, 1,
                     0, 0, 0, 0
                  };

  double c[4*4];

  double d[5*5];

//cblas_dgemv(CblasRowMajor, CblasNoTrans, 4,   5,   1,     a,   5, x, 1,    0,    y, 1); 

matrix_vector_mul(a, 4, 5, x, 5, y);

for (int i=0;i<4;i++) {
 printf("%d: %.2f\n", i, y[i]);
}

matrix_matrix_mul(a, 4, 5, b, 5, 4, c);

for (int j=0;j<4;j++) {
  for (int k=0;k<4;k++) {
	printf("%.2f ", c[4*j+k]);
}
	printf("\n");
}

matrix_matrix_mul(b, 5, 4, a, 4, 5, d);

for (int j=0;j<5;j++) {
  for (int k=0;k<5;k++) {
        printf("%.2f ", d[5*j+k]);
}
        printf("\n");
}


//for (int i=0;i<16;i++) {
// printf("%d: %.2f\n", i, c[i]);
//}


}
