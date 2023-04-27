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
			fprintf(fp, " (");
			if (t->params) param_list_fprint(fp, t->params); else fprintf(fp, "void"); fprintf(fp, ")");
			break;
  }
}
void type_print(struct type* t) { type_fprint(stdout, t); }

void type_subtype_leaf_assign(struct type* t, struct type* subtype) {
  if (!t->subtype) t->subtype = subtype;
  else type_subtype_leaf_assign(t->subtype, subtype);
}

void type_destroy(struct type** t) {
  if (!(*t)) return;
  param_list_destroy(&((*t)->params));
  type_destroy(&((*t)->subtype));
  expr_destroy(&((*t)->size));
  free(*t); *t = NULL;
}

struct type* type_copy(struct type* t) {
  if (!t) return NULL;
  struct type* copy = malloc(sizeof(*copy));
  if (copy) {
    copy->kind = t->kind;
    copy->params = param_list_copy(t->params);
    copy->subtype = type_copy(t->subtype);
    copy->size = expr_copy(t->size);
  }
  return copy;
}

bool type_equals(struct type* a, struct type* b) {
  if ((!a && b) || (a && !b)) return false;
  if (!a && !b) return true;
  bool ret = false;
  if (a->kind == b->kind) {
    if (a->kind && b->kind == TYPE_ARRAY) ret = type_equals(a->subtype, b->subtype);
    else if (a->kind && b->kind == TYPE_FUNCTION) ret = type_equals(a->subtype, b->subtype) && param_list_equals(a->params, b->params);
    else ret = true;
  }
  return ret;
}

int type_resolve(struct symbol_table* st, struct type* t) {
  if (!st || !t) return 0;
  return param_list_resolve(st, t->params) + type_resolve(st, t->subtype) + expr_resolve(st, t->size);
}

