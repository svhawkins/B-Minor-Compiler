#include <stdio.h>
#include <stdlib.h>
#include "param_list.h"

struct param_list* param_list_create(char* name, struct type* type, struct param_list* next )
{
  struct param_list* p = malloc(sizeof(*p));
  if (p) {
    p->name = name;
    p->type = type;
    p->next = next;
    p->symbol = NULL;
  }
  return p;
}

void param_list_fprint(FILE* fp, struct param_list* p) {
  if (!p) return;
  fprintf(fp, "%s: ", p->name); type_fprint(fp, p->type);
  if (p->next) fprintf(fp, ", "); // another item follows
  param_list_fprint(fp, p->next);
}

void param_list_print(struct param_list *p) { param_list_fprint(stdout, p); }

void param_list_destroy(struct param_list** p) {
  if (!(*p)) return;
  free((*p)->name);
  type_destroy(&((*p)->type));
  //symbol_destroy(&((*p)->symbol));
  param_list_destroy(&((*p)->next));
  free(*p); *p = NULL;
}

struct param_list* param_list_copy(struct param_list* p) {
  if (!p) return NULL;
  struct param_list* copy = malloc(sizeof(*copy));
  if (copy) {
    copy->name = strdup(p->name);
    copy->type = type_copy(p->type);
    copy->symbol = symbol_copy(p->symbol);
    copy->next = param_list_copy(p->next);
  }
  return copy;
}

bool param_list_equals(struct param_list* a, struct param_list* b) {
  if ((!a && b) || (a && !b)) return false; if (!a && !b) return true;
  bool current = !strcmp(a->name, b->name) && type_equals(a->type, b->type);
  return current && param_list_equals(a->next, b->next);
}
