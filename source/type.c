#include <stdio.h>
#include <stdlib.h>
#include "type.h"

struct type* type_create(type_t kind, struct type* subtype, struct param_list* params)
{
  struct type* t = malloc(sizeof(*t));
  if (t) {
    t->kind = kind;
    t->subtype = subtype;
    t->params = params;
  }
  return t;
}
void type_print(struct type* t) {}
