#include <stdio.h>
#include <stdlib.h>


typedef struct Expression Expression;

struct Expression {
  Expression* lv;
  Expression* rv;
  float value;
  char op;
}; 

Expression * wrap_float(float x);

int is_lc_char(char c) {
  return (c >= 'a' && c <= 'z');
}

Expression * new_expression() {
  return malloc(sizeof(Expression));
}

Expression * copy(Expression *x) {
  Expression * ret = new_expression();
  ret->lv = x->lv;
  ret->rv = x->rv;
  ret->op = x->op;
  ret->value = x->value;
  return ret;
}

Expression * bop(Expression *x, Expression *y, char op) {
  Expression * ret = new_expression();
  ret->lv = copy(x);
  ret->rv = copy(y);
  ret->op = op;
  return ret;

}

Expression * add(Expression *x, Expression *y) {
  if (x->op == '@' && x->value==0) {return copy(y);}
  if (y->op == '@' && y->value==0) {return copy(x);}
  return bop(x,y,'+'); 
  
}

Expression * mul(Expression *x, Expression *y) {
  if (x->op == '@' && x->value==0) {return wrap_float(0);}
  if (y->op == '@' && y->value==0) {return wrap_float(0);}	
  if (x->op == '@' && x->value==1) {return copy(y);}
  if (y->op == '@' && y->value==1) {return copy(x);}

  return bop(x,y,'*'); 
}

Expression * sub(Expression *x, Expression *y) {
  return add(x,mul(wrap_float(-1),y));
}

Expression * divx(Expression *x, Expression *y) {
  if (x->op == '@' && x->value==0) {return wrap_float(0);}
  if (y->op == '@' && y->value==0) {printf("Divide by 0\n"); exit(-1);}
  if (y->op == '@' && y->value==1) {return copy(x);}
  return bop(x,y,'/');
}

Expression * powx(Expression *x, Expression *y){
  if (x->op == '@' && x->value==1) {return copy(x);}
  if (y->op == '@' && y->value==0) {return wrap_float(1);}
  if (y->op == '@' && y->value==1) {return copy(x);}
  return bop(x,y,'^');
}

Expression * wrap_float(float x) {

  Expression * ret = new_expression();
  ret->value = x;
  ret->op = '@';
  return ret;
}

Expression * wrap_var(char x) {

  Expression * ret = new_expression();
  ret->op = x;
  return ret;

}

Expression* derive(Expression *e, char v) {

  Expression * ret = new_expression();
  
  if (e->op == '@') {
    ret->op = '@';
    ret->value = 0;
  }
  else if (e->op == v) {
    ret->op = '@';
    ret->value = 1;
  }
  else if (is_lc_char(e->op)) { // any variable other than wrt differentiation
    ret->op = '@';
    ret->value = 0;
  } 
  else if (e->op == '+') {
    ret = add(derive(e->lv,v),derive(e->rv,v));
  }
  else if (e->op == '*') {
    ret = add( mul(e->lv,derive(e->rv,v)),
               mul(e->rv,derive(e->lv,v)));
  }
  else if (e->op == '/') {
    ret = divx( sub( mul(e->rv,derive(e->lv,v)),
               mul(e->lv,derive(e->rv,v))),
               mul(e->rv,e->rv));
  }
  else if (e->op == '^') {
    if (e->rv->op == '@') {
      ret = mul(mul(e->rv,powx(e->lv,wrap_float(e->rv->value - 1))),
                derive(e->lv,v));
    }
    else {
      printf("Exponentiation other than constants not implemented\n");
      exit(-1);
    }
  }
 

  else {
    printf("Not implemented: %c\n", e->op);
    exit(-1);
  }

  return ret;       

}

int is_leaf(Expression * e) {
  return (e->lv->op=='@' || is_lc_char(e->lv->op));
}

void print(Expression *e,int d) {
 //if (d>5) return;
  if (e->op == '@') {
    printf("%.1f ", e->value);
    return;
  }

  if (is_lc_char(e->op)) {
    printf("%c ", e->op);
    return;
  }

  int lvx = !is_leaf(e);
  int rvx = !is_leaf(e);

  if (lvx) printf("( ");
  print(e->lv,d+1);
  if (lvx) printf(") ");

  printf("%c ", e->op);

  if (rvx) printf("( ");
  print(e->rv,d+1);
  if (rvx) printf(") ");

 
}


int main () {


Expression *a = wrap_float(10);
Expression *b = wrap_float(20);
Expression *c = wrap_float(30);

Expression *t = wrap_var('x');

Expression *q = add(a,b);

Expression *d = add(t,q);

//printf("%c\n", c.op);
//printf("%c\n", q.op);
//printf("%c\n", a.op);

print(d,0);
printf("\n");
print(derive(d,'x'),0);
printf("\n");

Expression * lexp = add(add(mul(wrap_var('x'),wrap_var('x')),
               mul(wrap_float(2),wrap_var('x'))),
           wrap_float(1));

print(lexp, 0); printf("\n");
print(derive(lexp,'x'), 0); printf ("\n");

//Expression * pp = divx(wrap_var('x'),powx(add(wrap_var('x'),wrap_float(3)),wrap_float(2)));

Expression * pp = divx(wrap_var('x'), powx(add(powx(wrap_var('x'),wrap_float(2)), wrap_float(1)), wrap_float(2)));

print(pp,0); printf("\n");
print(derive(pp,'x'),0); printf("\n");

Expression * qq = divx(wrap_var('y'), powx(add(powx(wrap_var('x'),wrap_float(2)), wrap_float(1)), wrap_float(2)));

print(qq,0); printf("\n");
print(derive(qq,'x'),0); printf("\n");
print(derive(derive(qq,'x'),'y'),0); printf("\n");


//for (char c='A';c<='z';c++) {
//printf("%c %s a lowercase char\n", c, is_lc_char(c)?"is":"isn't");
//}

}
