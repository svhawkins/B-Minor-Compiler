#include <stdio.h>
#include <stdlib.h>
#include "type.h"

struct type* type_create(type_t kind, struct type* subtype, struct param_list* params, struct expr* size)
{
  struct type* t = malloc(sizeof(*t));
  if (t) {
    t->kind = kind;
    t->subtype = subtype;
    t->params = NULL;
    t->size = size;

    // only function declarations have parameter lists
    if (t->kind == TYPE_FUNCTION) t->params = params;
  }
  return t;
}

void type_fprint(FILE* fp, struct type* t) {
  if (!t) return;
  switch(t->kind) {
    // base cases
    case TYPE_INTEGER: fprintf(fp, "integer"); break;
    case TYPE_BOOLEAN: fprintf(fp, "boolean"); break;
    case TYPE_CHARACTER: fprintf(fp, "char"); break;
    case TYPE_STRING: fprintf(fp, "string"); break;
    case TYPE_VOID: fprintf(fp, "void"); break;
    case TYPE_AUTO: fprintf(fp, "auto"); break;

    // recursive cases
    case TYPE_ARRAY: fprintf(fp, "array ["); expr_fprint(fp, t->size); fprintf(fp, "] ");
		     type_fprint(fp, t->subtype);
		     break;
    case TYPE_FUNCTION: fprintf(fp, "function "); type_fprint(fp, t->subtype);
			fprintf(fp, " ("); if (t->params) param_list_fprint(fp, t->params); else fprintf(fp, "void"); fprintf(fp, ")");
			break;
  }
}
void type_print(struct type* t) { type_fprint(stdout, t); }
