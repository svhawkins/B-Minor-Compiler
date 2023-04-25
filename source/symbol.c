#include <stdio.h>
#include <stdlib.h>
#include "symbol.h"


struct symbol* symbol_create(symbol_t kind, struct type* type, char* name) {
  struct symbol* sym = malloc(sizeof(*sym));
  if (sym) {
    sym->kind = kind;
    sym->name = name;
    sym->type = type;
   // add to symbol table?
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
  fprintf(fp, "(kind: %d, name: %s, type: ", s->kind, s->name);
  type_fprint(fp, s->type);
  fprintf(fp, ")\n");
}
