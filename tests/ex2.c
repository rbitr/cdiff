#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "blas/matmul.h"


void print_array(void * A, int rows, int cols, char f) {

char * s;

double * B = (double *)A;

for (int r=0;r<rows;r++) {
    for (int c=0;c<cols;c++) {
	s = ", ";
	if (c==(cols-1)) s = "\n";
	if (f=='f')
	    printf("%f%s", ((double *) A)[c+cols*r], s);
	else if (f=='d')
           printf("%d%s", ((int *) A)[c+cols*r], s);		
}    
}
}

double sigmoid(double x) {

    return 0.5 * tanh(5* x) + 0.5;

}

int int_sum(int * v, int l) {

int res = 0;
while(l) {
res+=v[--l]; // hopefully that's right
}
return res;
}

double log_loss(double * preds, int * targs, int l) {
double p;
	double res =0;
	while(l--) {
          p = preds[l]*targs[l] + (1-preds[l])*(1-targs[l]);
	  res += log(p);
	}

	return -res;
}

double * d_log_loss(double * preds, int * targs, int l) {

	// this needs to be a vector I guess
	double * res = malloc(l*sizeof(double));
	double y, t;
	while(l--) {
           	y = preds[l];
		t = targs[l];
		res[l] = -t/y + (1-t) / ((1-y)+.000001);
	}

	return res;
}

double * d_sigmoid(double *s, int l) {

	double * res = malloc(l*sizeof(double));

	while(l--) {
	 
		res[l] = 0.5 * 1 /(cosh(s[l]) * cosh(s[l]));

	}

	return res;

}





int main(void)
{
    FILE * fp;
    char * line = NULL;
    size_t len = 0;
    ssize_t read;
    char * token; 

    int data_cols = 2;
    int data_rows = 100;
    double * inputs = malloc(data_rows*data_cols*sizeof(double));    
    int * targets = malloc(data_rows*sizeof(int));

    double w[2] = {1, 1};
    double b = 0;

    // READ IN SOME NUMBERS
    fp = fopen("./data/rand_100_2.txt", "r");
    if (fp == NULL)
        exit(EXIT_FAILURE);

    for (int ix=0;(read = getline(&line, &len, fp)) != -1;) {
        //printf("Retrieved line of length %zu:\n", read);
	while ((token = strsep(&line, ","))) { 
            //printf("%f\n", strtof(token,NULL));
            inputs[ix] = strtof(token,NULL);
            ix++;
        }

    }
    fclose(fp);
    if(line) free(line);

    // TODO: Make into a fake classification problem
    // they're just some random numbers, add .5,.5 to half, subtract from half
    // and make two classes
    double offset = .6;
    for (int ix=0;ix<data_rows;ix++) {
	if (ix<(data_rows/2)) {
	    inputs[ix*data_cols]-=offset;
	    inputs[ix*data_cols+1]-=offset;
	    targets[ix]=0;
	}
	else {
	    inputs[ix*data_cols]+=offset;
	    inputs[ix*data_cols+1]+=offset;
	    targets[ix]=1;
	}
    }
 


    //


    
    // compute logits
    // y = W*x + b
    // probabilities are sigmoid(y)
    double y0[100];
    double y1[100];
    int pred[100];
    int acc[100];

    int acc_sum;
    double loss;
    double accuracy;

    double * dldy;

    double * dyds;

    double * dldw = malloc(data_cols*sizeof(double));
    double * dldb = malloc(sizeof(double));


    int max_steps = 100;
    double alpha = 0.001;

    for (int step=0;step<max_steps;step++) {

	// get the prediction
        matrix_matrix_mul(inputs, 100, 2, w, 2, 1, y0);

        for (int i=0;i<100;i++) {
   	 y0[i] += b;
    	y1[i] = sigmoid(y0[i]);
    	pred[i] = (int)(y1[i]+.5);
    	acc[i] = !(pred[i]^targets[i]);
    }

	// get the loss

	int acc_sum;
	acc_sum = int_sum(acc, data_rows);

    	accuracy = (acc_sum + 0.0) / data_rows;

    	loss = log_loss(y1, targets, data_rows);
	 
        printf("step %d accuracy: %f, loss %f; ", step, accuracy, loss);

	// get gradients
	 dldy = d_log_loss(y1, targets, data_rows);

         dyds = d_sigmoid(y0, data_rows);


	    for (int i=0;i<data_cols;i++) {
        dldw[i] = 0;
	}

    	dldb[0] = 0;

    	double dsdwi;
    	double dsdbi;
    	for (int l=0;l<data_rows;l++) {


        	dsdbi = 1;
        	dldb[0] += dldy[l]*dyds[l]*dsdbi;

        	for (int k=0;k<data_cols;k++ ) {
          	dsdwi = inputs[l*data_rows+k];
          	dldw[k] += dldy[l]*dyds[l]*dsdwi;

        }

        }
// this shouldn't be commented
//        free(dldy);
//	free(dyds);

	for (int i=0;i<data_cols;i++) {
	     printf("dW%d: %f, ", i, dldw[i]);
	}	

    	printf("db: %f, ", dldb[0]);


	//update

	for (int c=0;c<data_cols;c++) {
	w[c] -= alpha * dldw[c];
	}
	b -= alpha * dldb[0];
	
	 for (int i=0;i<data_cols;i++) {
             printf("W%d: %f, ", i, w[i]);
        }

        printf("bb: %f, ", b);
 
	printf("\n");

    }


   


 
    free(inputs);
    exit(EXIT_SUCCESS);
}


