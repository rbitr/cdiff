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

int is_zero(Expression * x) {
  return x->op == '@' && x->value==0; // needs to be factored out
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
    //if (e->rv->op == '@') {
    // see if the d(exponent)/dv == 0
    Expression * de = derive(e->rv,'v');
    // 
    if (is_zero(de)) { // need to be modified to deal with exponent expression == 0 
      ret = mul(mul(e->rv,powx(e->lv,add(e->rv, wrap_float(- 1)))),
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
  return (e->op=='@' || is_lc_char(e->op));
}

void _print(Expression *e) {
 //if (d>5) return;
  if (e->op == '@') {
    printf("%.1f ", e->value);
    return;
  }

  if (is_lc_char(e->op)) {
    printf("%c ", e->op);
    return;
  }

  int lvx = !is_leaf(e->lv);
  int rvx = !is_leaf(e->rv);

  if (lvx) printf("( ");
  _print(e->lv);
  if (lvx) printf(") ");

  printf("%c ", e->op);

  if (rvx) printf("( ");
  _print(e->rv);
  if (rvx) printf(") ");

 
}

void print(Expression * e) {
  _print(e); printf("\n");
}


int main () {

printf("Tests: \n");

Expression *a = wrap_float(10);
Expression *b = wrap_float(20);
Expression *c = wrap_float(30);

Expression *t = wrap_var('x');

Expression *q = add(a,b); // 10 + 20

print(q);

Expression *d = add(t,q); // x + 10 + 20

print(d);

print(derive(d,'x')); // 1

Expression * lexp = add(add(mul(wrap_var('x'),wrap_var('x')),
               mul(wrap_float(2),wrap_var('x'))),
           wrap_float(1)); //x^2 + 2x + 1

print(lexp); 
print(derive(lexp,'x')); // 2x + 2

Expression * mm = divx( wrap_var('y'), add(wrap_var('x'), wrap_float(1)));

print(mm);
print(derive(mm,'x'));
print(derive(mm,'y'));

Expression *nn = divx( powx( add (wrap_var('x'), wrap_float(1)), wrap_float(2)),
                       powx( sub (wrap_var('x'), wrap_float(1)), wrap_float(2))
                     );

print(nn);
print(derive(nn,'x'));
print(derive(nn,'y'));

Expression * x = wrap_var('x');
Expression * y = wrap_var('y');
Expression * _1 = wrap_float(1);
Expression * _2 = wrap_float(2);

Expression *oo = powx( add (powx(x,_2), _1), add(y,_1));

print(oo);
printf("%d\n",is_leaf(_1));

print(derive(oo, 'x'));

// fails to recognize exponent == 0
Expression *ss = powx(x,_2);
print(derive(ss,'x'));
print(derive(derive(ss, 'x'), 'x'));
print(derive(derive(derive(ss,'x'), 'x'), 'x'));

exit(0);

//Expression * pp = divx(wrap_var('x'),powx(add(wrap_var('x'),wrap_float(3)),wrap_float(2)));

Expression * pp = divx(wrap_var('x'), powx(add(powx(wrap_var('x'),wrap_float(2)), wrap_float(1)), wrap_float(2)));

print(pp); printf("\n");
print(derive(pp,'x')); printf("\n");

Expression * qq = divx(wrap_var('y'), powx(add(powx(wrap_var('x'),wrap_float(2)), wrap_float(1)), wrap_float(2)));


print(qq); printf("\n");
print(derive(qq,'x')); printf("\n");
print(derive(derive(qq,'x'),'y')); printf("\n");



}
