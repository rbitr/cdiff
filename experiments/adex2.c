#include <stdio.h>
#include <stdlib.h>

//#define _nad(x,y) wrapped_fun(nadd,x,y)
//
//#define nadd(x,y) _nad(x,y)
//#define _nmul(x,y) wrapped_fun(nmul,x,y)
//#define nmul(x,y) _nmul(x,y)

#define NADD(x,y) wrapped_fun(nadd, x, y)
#define NMUL(x,y) wrapped_fun(nmul, x, y)


float myfun (float x, float c) {
	printf("x is now %f\n", x);
	for (int ix=0;ix<5;ix++) {
		x = x*x + c;
		printf("x is now %f\n", x);
	}
	return x;
}

typedef struct node node;

struct node {
	float value;
	node ** parents;
	node * (* func)(node *, node *);
};

float fadd(float x, float y) { return x+y; };
float fmul(float x, float y) { return x*y; };

node * wrapped_fun( node * (* func)(node *, float), node * a, float b) { 
// the nodes at the end of the function are the parents... should be varadic
	
	node * ans = malloc(sizeof(node));
	ans->parents = malloc(sizeof(node *)); // however many
	ans->parents[0] = a;
	
	ans->func = func;
	ans->value = func(a,b)->value;

	return ans;

}

node * add_const(node *x, float c) {
	node *r = malloc(sizeof(node));
        r->value = x->value + c;
        return r;
}

node *  pow_int(mode *x, float c) {
	node *r = malloc(sizeof(node));
        r->value=1;
	for (int i=0;i<c;i++) {
		r->value = r->value * x->value;
	}        
	return r;	
}

node * mul_const(node *x, float c) {
        node *r = malloc(sizeof(node));
        r->value = x->value * c;
        return r;
}


node * nadd(node *x, node *y) {
	node *r = malloc(sizeof(node));
	r->value = x->value + y->value;
	return r;
}

node * nmul(node *x, node *y) {
        node *r = malloc(sizeof(node));
        r->value = x->value * y->value;
        return r;
}

float myfun2 (float x, float c) {
        printf("x is now %f\n", x);
        for (int ix=0;ix<5;ix++) {
                float tmp = fmul(x,x);
                x = fadd(tmp, c);
                printf("x is now %f\n", x);
               }
        return x;
}


float myfun3 (float x, float c) {
        printf("x is now %f\n", x);
        for (int ix=0;ix<5;ix++) {
                float tmp = fmul(x,x);
                x = fadd(tmp, c);
                printf("x is now %f\n", x);
               }
        return x;
}

node * myfun4 (node *x, float c) {

	printf("x is now %f\n", x->value);
        for (int ix=0;ix<5;ix++) {
                node * tmp = POW(x,2);
		free(x);
                x = ADD_CONST(tmp, c);
		free(tmp);
                printf("x is now %f\n", x->value);
               }
        return x;
	
}

int main () {
	
	float x, c, v;
	x = .1; 

	c = 1; 
	v = myfun3(x,c); 

	node *a, *b;

	a = malloc(sizeof(node));
	b = malloc(sizeof(node));

	a->value = x;
	b->value = c;
	a->parents = NULL;
	b->parents = NULL;

	node *d = myfun4(a,c);	

	printf("%f %f\n", d->parents[0]->value, d->parents[1]->value);

	return 0;


	
}

