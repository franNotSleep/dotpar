#include <stdio.h>
#include <stdlib.h>

typedef struct Var {
  int n;
  struct Var **vars;
} Var;

int main() {
  Var *v1 = (Var *)malloc(sizeof(Var));
  v1->n = 10;

  Var *v2 = (Var *)malloc(sizeof(Var));
  v2->n = 15;

  Var *v3 = (Var *)malloc(sizeof(Var));
  v3->n = 20;

  Var *v = (Var *)malloc(sizeof(Var));
  v->vars = (Var **)malloc(sizeof(Var *) * 3);
  v->vars[0] = v1;
  v->vars[1] = v2;
  v->vars[2] = v3;

  for (int i = 0; i < 3; i++) {
    printf("%p %d %d\n", v->vars[i], (*(*((*v).vars + i))).n, v->vars[i]->n);
  }
}
