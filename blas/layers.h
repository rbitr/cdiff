#include "matmul.h"


//void dense () {
//
//}

typedef struct Dense Dense;

struct Dense {
double *W, *b;
unsigned input_dim, output_dim;

void (* apply)(Dense * self, double *X, int m, int k, double *Y);

}; 

void dense_apply(Dense * self, double *X, int m, int k, double *Y) {
	
	// input_dim = k; output_dim = n
	matrix_matrix_mul(X, m, k, self->W, self->input_dim, self->output_dim, Y);
	// b has the dimension n (for a 1d input)
	// we need to add this to every row (m of them) of Y
	for (int i=0;i<m;i++) { 
              for (int j=0;j<self->output_dim;j++) {
		Y[i*self->output_dim+j] += self->b[j];
	}}
} 

Dense * dense(double * W, int id, double * b, int od) {

  Dense * dl1 = malloc(sizeof(Dense));
  dl1->apply = dense_apply;

  dl1->W = W;
  dl1->input_dim = id;
  
  dl1->b = b;
  dl1->output_dim = od;

  return dl1;;

}

void jac_dense(Dense * D, double * X, int m, int n, double * dx, double *Y) {
// try this
// it could also return a layer that acts as the jacobian?
if (dx == X) {
 return D->W;
}
else if (dx == D->W) {
 return X; // need to transpose, let's see how it will work first
}
else if (dx == D->b) {
 // identity matrix
}  
else {
  // return zeros
}
}

typedef struct Relu Relu; 

struct Relu
{
// is there a reason it would be better to 
// keep the dimensions with the struct?
int m, n; 
void (* apply)(Relu * self, double *x, double *y);

};


double scalar_relu(double x) {
if (x > 0) return x;
return 0;
}




void apply_relu(Relu * self, double *x, double *y) {

for (int i=0;i<(self->m*self->n);i++) {
  y[i] = scalar_relu(x[i]); 
}

}


Relu * relu(int m, int n) {
Relu * r = malloc(sizeof(Relu));
r->m = m;
r->n = n;
r->apply = apply_relu;
return r;

}

/* 

        matrix_matrix_mul(inputs, 100, 2, w, 2, 1, y0);

        for (int i=0;i<100;i++) {
         y0[i] += b;
        y1[i] = sigmoid(y0[i]);
        pred[i] = (int)(y1[i]+.5);
        acc[i] = !(pred[i]^targets[i]);
    }

*/


