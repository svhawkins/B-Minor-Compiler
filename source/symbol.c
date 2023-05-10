#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symbol.h"


struct symbol* symbol_create(symbol_t kind, struct type* type, char* name) {
  struct symbol* sym = malloc(sizeof(*sym));
  if (sym) {
    sym->kind = kind;
    sym->name = name;
    sym->type = type;
    sym->defined = false;
    sym->which = -1;
  }
  return sym;
}


void symbol_destroy(struct symbol** s) {
  if (!s || !(*s)) return;
  type_destroy(&((*s)->type));
  free((*s)->name);
  free(*s); *s = NULL;
}

void symbol_fprint(FILE* fp, struct symbol* s) {
  char str[10];
    switch(s->kind) {
    case SYMBOL_GLOBAL: strcpy(str, "global"); break;
    case SYMBOL_LOCAL: strcpy(str, "local"); break;
    case SYMBOL_PARAM: strcpy(str, "parameter"); break;
  }
  fprintf(fp, "(kind: %s, name: %s, type: ", str, s->name);
  type_fprint(fp, s->type);
  if (s->which >= 0) { fprintf(fp, ", which: %d", s->which); }
  fprintf(fp, ")\n");
}

struct symbol* symbol_copy(struct symbol* s) {
  if (!s) return NULL;
  struct symbol* copy = malloc(sizeof(*copy));
  if (copy) {
    copy->kind = s->kind;
    copy->which = s->which;
    copy->name = strdup(s->name);
    copy->type = type_copy(s->type);
  }
  return copy;
}
