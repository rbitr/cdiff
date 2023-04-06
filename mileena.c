#include <stdio.h>
#include <stdlib.h>

#define DEBUG_ON

#define MAX_NODES 50

typedef struct node node;

struct node {
	float value;
	node ** parents;
	unsigned parent_count;
	node * (* func)(node *, node *);
};

char * lookup_reg_fun (void * f);

int isprimitive(void * x);

float fadd(float x, float y) { return x+y; };
float fmul(float x, float y) { return x*y; };

node * wrapped_fun( node * (* func)(node *, node *), node * a, node *b) { 
// the nodes at the end of the function are the parents... should be variadic
	
	// if the function func is itself wapped, just run it?
	// otherwise assign parents manuallyt
	node * ans;
	if  (!isprimitive(func)) {
		#ifdef DEBUG_ON
		printf("passing it on\n");
		#endif
		ans = func(a,b);

	}
	else {

	ans = malloc(sizeof(node));
	ans->parent_count = 2;	
	ans->parents = malloc(2*sizeof(node *)); // however many
	ans->parents[0] = a;
	ans->parents[1] = b;
	
	ans->func = func;
	ans->value = func(a,b)->value;
	}
	#ifdef DEBUG_ON
	printf("wrapped %s: %llu\n", lookup_reg_fun(func), (long long) func);
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


// obviously a hack for now
// I think that only primitives need to be registred
int isprimitive(void *x ) {

	if ((x==nadd) || (x==nmul)) return 1;

	return 0; 

}

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

char * lookup_reg_fun (void * f)  {

	for (int i=0;i<fnum;i++) {
	if (freg[i].ptr == f) return freg[i].name;
	}	
	return "Function not found";

}

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
                //lookup_reg_fun(end->func);
		printf("%llu\n", (long long) end->func);
	        printf("%s\n", lookup_reg_fun(end->func));        
		end = end->parents[0];
		#ifdef DEBUG_ON
		printf("%llu\n", (long long) end);
		#endif
                }

        return;
}

typedef struct linear_dict linear_dict;

struct linear_dict  {
	void * keys[MAX_NODES];
	int values[MAX_NODES];	
	int len;
};

int linear_dict_indexof(linear_dict *d, void *k) {
	for (int i=0;i<d->len;i++) {
		if (d->keys[i] == k) {
			return i;
		}
	}
	return -1;
}



void linear_dict_set(linear_dict * d, void *k, int v) {
	int ix = linear_dict_indexof(d,k);	
	if (ix >=0) {
		d->values[ix] = v;
	}
	else {
		d->keys[d->len] = k;
		d->values[d->len] = v;
		d->len++;
	} 	
}

int linear_dict_get(linear_dict * d, void *k) {
        int ix = linear_dict_indexof(d,k);
        if (ix >=0) {
                return d->values[ix];
        }
        else {

                d->keys[d->len] = k;
                d->values[d->len] = 0;
		d->len++;	
		return 0;
        }
}



// translated from https://github.com/HIPS/autograd/blob/master/autograd/util.py
node ** toposort(node *end_node) {
	
	node ** sorted = malloc(MAX_NODES * sizeof(node));
	int sortptr = -1;

	node ** stack = malloc(MAX_NODES * sizeof(node));
	stack[0] = end_node;
	int stackptr = 0;
	
	linear_dict * child_counts = malloc(sizeof(linear_dict));
	child_counts->len = 0;
	
	while (stackptr >= 0) {
		node * cur  = stack[stackptr];
		stackptr--;
		int c = linear_dict_get(child_counts, cur);	
		if (c) {
			linear_dict_set(child_counts, cur,c + 1);
		}
		else {
			linear_dict_set(child_counts, cur,1);
			// add node parents to stack	
			for (int p=0;p<cur->parent_count;p++) {
				stackptr++;
				stack[stackptr] = cur->parents[p];
			}
		}	
		
	}

	#ifdef DEBUG_ON
	for (int k=0;k<child_counts->len;k++) {
		printf("%d, %llu, %d\n", k, 
			(long long) child_counts->keys[k],
			child_counts->values[k]);
	}
	#endif		
	
	// reuse stack as childless nodes
	stack[0] = end_node;
	stackptr=0;

	while (stackptr >= 0) {
		node * cur  = stack[stackptr];
                stackptr--;
		sortptr++;
		sorted[sortptr]=cur;
	
                for (int p=0;p<cur->parent_count;p++) {
        		int co = linear_dict_get(child_counts, cur->parents[p]);
			if (co<1) {
				printf("error, shouldn't be any with no children");
				exit(-1);
			}                
			else if (co==1) {
				stackptr++;
				stack[stackptr] = cur->parents[p];
			}
			else {
				linear_dict_set(child_counts, cur->parents[p],
						co-1);		
			} //if
		} // for


	} //while

	#ifdef DEBUG_ON
	printf("remaining node counts\n");
	for (int k=0;k<child_counts->len;k++) {
                printf("%d, %llu, %d\n", k,
                        (long long) child_counts->keys[k],
                        child_counts->values[k]);
        }
	

	printf("Sorted graph\n");
	for (int k=0;k<=sortptr;k++) {
		printf("%d, %llu\n", k, (long long) sorted[k]);
	}
	#endif


	sorted[sortptr+1] = 0; //unfortunate
	return sorted;	
}

int main () {

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
	a->parent_count = 0;
	b->parent_count = 0;


	printf("%llu\n", (long long) a);
	node *d = myfun4(a,b);	

	printf("%f %f\n", d->parents[0]->value, d->parents[1]->value);

	print_backtrace(d,a);

	toposort(d);

	return 0;


	
}

