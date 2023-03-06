// basic matrix multiplication routines using blas
#include <stdio.h>
#include <stdlib.h>
#include "matmul.h"

#include "layers.h"

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
                     0, -1, 0, 0,
                     0, 0, 0, -1,
                     0, 0, 0, 0
                  };

  double c[4*4];

  double d[5*5];

  double z[5*2] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10 }; 
  double k[3*2] = {1, 5, };

  double r[5];

  matrix_matrix_mul(z, 5, 2, k, 2, 1, r);
 // matrix_vector_mul(z, 5, 2, k, 2, r);
  
 //  cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, 5,    2,    3, 1.0,   z,   5, k, 3,  0.0, r,  5); 
  printf("r = w*x\n");
  for (int l=0;l<5;l++ ){ printf ("!%f\n", r[l]); }

//  Dense * dl1 = malloc(sizeof(Dense));
//  dl1->apply = dense_apply;

 // dl1->W = k;
  //dl1->input_dim = 2;

  double dl1b[1] = {1};
 // dl1->output_dim = 1;

  Dense * dl1 = dense(k, 2, dl1b, 1);

 // dl1->b = dl1b;

  dl1->apply(dl1, z, 5, 2, r);

  printf("r = w*x + 1\n");
  for (int l=0;l<5;l++ ){ printf ("!%f\n", r[l]); }




matrix_vector_mul(a, 4, 5, x, 5, y);

for (int i=0;i<4;i++) {
 printf("%d: %.2f\n", i, y[i]);
}

double dlb2[4] = {1, 2, 3, 4};


matrix_matrix_mul(a, 4, 5, b, 5, 4, c);


printf("a * b\n");
for (int j=0;j<4;j++) {
  for (int k=0;k<4;k++) {
	printf("%.2f ", c[4*j+k]);
}
	printf("\n");
}

Dense * dl2 = dense(b, 5, dlb2, 4);

dl2->apply(dl2, a, 4, 5, c);

printf("a * b + {1, 2, 3, 4}\n");
for (int j=0;j<4;j++) {
  for (int k=0;k<4;k++) {
        printf("%.2f ", c[4*j+k]);
}
        printf("\n");
}

Relu * rr = relu(4, 4);
rr->apply(rr, c, c);

printf("relu(.)\n");
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
