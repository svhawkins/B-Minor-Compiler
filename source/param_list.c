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

   // TO DO: add to symbol table
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
  symbol_destroy(&((*p)->symbol));
  param_list_destroy(&((*p)->next));
  free(*p); *p = NULL;
}
