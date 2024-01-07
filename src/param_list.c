#include <stdio.h>
#include <stdlib.h>
#include "param_list.h"

struct param_list* param_list_create(char* name, struct type* type, struct param_list* next )
{
  struct param_list* p = malloc(sizeof(struct param_list));
  if (p) {
    p->name = name;
    p->type = type;
    p->next = next;
    p->symbol = NULL;
  }
  return p;
}

void param_list_fprint(FILE* fp, struct param_list* p) {
  if (!p) { return; }
  fprintf(fp, "%s: ", p->name); type_fprint(fp, p->type);
  if (p->next) { fprintf(fp, ", "); } // another item follows
  param_list_fprint(fp, p->next);
}

void param_list_print(struct param_list *p) { param_list_fprint(stdout, p); }

void param_list_destroy(struct param_list** p) {
  if (!(*p)) { return; }
  free((*p)->name);
  type_destroy(&((*p)->type));
  param_list_destroy(&((*p)->next));
  free(*p); *p = NULL;
}

struct param_list* param_list_copy(struct param_list* p) {
  if (!p) { return NULL; }
  struct param_list* copy = malloc(sizeof(struct param_list));
  if (copy) {
    copy->name = strdup(p->name);
    copy->type = type_copy(p->type);
    copy->symbol = symbol_copy(p->symbol);
    copy->next = param_list_copy(p->next);
  }
  return copy;
}

bool param_list_equals(struct param_list* a, struct param_list* b) {
  if ((!a && b) || (a && !b)) { return false; }
  if (!a && !b) { return true; }
  bool current = !strcmp(a->name, b->name) && type_equals(a->type, b->type);
  return current && param_list_equals(a->next, b->next);
}

int param_list_resolve(struct symbol_table* st, struct param_list* p) {
  if (!st || !p) { return error_status; }
  struct symbol* s = symbol_create(SYMBOL_PARAM, type_copy(p->type), strdup(p->name));
  type_resolve(st, p->type); // parameters can be functions with their own parameters, put 'em in the table

  struct symbol* already_used_sym = symbol_table_scope_lookup_current(st, p->name);
  if (already_used_sym) {
    error_status = symbol_table_error_handle(SYM_REDEF, (void*)s, (void*)already_used_sym);
    symbol_destroy(&s);
  } else {
    p->symbol = s;
    symbol_table_scope_bind(st, p->name, p->symbol);
  }
  return param_list_resolve(st, p->next);
}
