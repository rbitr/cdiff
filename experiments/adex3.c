#include <stdio.h>
#include <stdlib.h>

#define DEBUG_ON

typedef struct node node;

struct node {
	float value;
	node ** parents;
	node * (* func)(node *, node *);
};

float fadd(float x, float y) { return x+y; };
float fmul(float x, float y) { return x*y; };

node * wrapped_fun( node * (* func)(node *, node *), node * a, node *b) { 
// the nodes at the end of the function are the parents... should be variadic
	
	node * ans = malloc(sizeof(node));
	ans->parents = malloc(2*sizeof(node *)); // however many
	ans->parents[0] = a;
	ans->parents[1] = b;
	
	ans->func = func;
	ans->value = func(a,b)->value;

	#ifdef DEBUG_ON
	printf("wrapped %llu\n", func);
	#endif

	return ans;

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

#define nadd(x,y) wrapped_fun(nadd, x, y)
#define nmul(x,y) wrapped_fun(nmul, x, y)

node * myfun4 (node *x, node *c) {
	node *r = x;//= malloc(sizeof(node));
	// r = nodecopy(x); 
	//r->value = x->value;
	//r->parents = x->parents;
	//r->func = r->func;
	printf("x is now %f\n", r->value);
        for (int ix=0;ix<5;ix++) {
                node * tmp = nmul(r,r);
		//free(r);
                r = nadd(tmp, c);
		// free(tmp);
                printf("x is now %f\n", r->value);
               }
        return r;
	
}
#define myfun4(x,y) wrapped_fun(myfun4, x, y)

void print_backtrace(node *end, node *start) {
	// trace argument 0, i.e. a
	printf("Backtrace:\n");
	printf("%llu %llu\n", (long long) end, (long long) start); 
	while ((end != start) && (end != NULL)) {
		if (end->parents[0] == NULL) {
			printf("Output unrelated to input\n");
			exit(-1);
		}
		//printf("loop\n");	
		printf("%llu\n", (long long) end->func);
		end = end->parents[0];
		}	

	return;
}

typedef struct named_func named_func;

struct named_func {
 void * ptr;
 char* name;

};

named_func freg[20];
size_t fnum = 0;

#define reg_fun(x) \
	freg[fnum].ptr = x;\
	freg[fnum].name = #x;\
	fnum++;


int main () {

	//named_func freg[20];
	//size_t fnum = 0;
	reg_fun(nadd);
	reg_fun(nmul);
	reg_fun(myfun4);
	
	printf("Registered functions:\n");
	for (int j=0;j<fnum;j++) {
		printf("%s: %llu\n", freg[j].name, (long long) freg[j].ptr);
	}
	
	
	float x, c, v;
	x = .1; 

	c = 1; 

	node *a, *b;

	a = malloc(sizeof(node));
	b = malloc(sizeof(node));

	a->value = x;
	b->value = c;
	a->parents = NULL;
	b->parents = NULL;


	printf("%llu\n", (long long) a);
	node *d = myfun4(a,b);	

	printf("%f %f\n", d->parents[0]->value, d->parents[1]->value);

	print_backtrace(d,a);

	return 0;


	
}

