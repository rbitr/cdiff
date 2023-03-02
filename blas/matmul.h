// basic matrix multiplication routines using blas
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
cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, rowsa,    colsb,    colsa, 1.0,   A,   colsa, B, colsb,  0.0, C,  colsb);

}

