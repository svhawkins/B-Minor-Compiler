#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symbol.h"

struct symbol* symbol_create(symbol_t kind, struct type* type, char* name) {
  struct symbol* sym = malloc(sizeof(struct symbol));
  if (sym) {
    sym->kind = kind;
    sym->name = (name != NULL) ? strdup(name) : NULL;
    sym->type = type;
    sym->defined = false;
    sym->which = -1;
    sym->address = NULL;
  }
  return sym;
}


void symbol_destroy(struct symbol** s) {
  if (!s || !(*s)) { return; }
  type_destroy(&((*s)->type));
  free((*s)->name);
  if ((*s)->address) free((*s)->address);
  free(*s); *s = NULL;
}

void symbol_fprint(FILE* fp, struct symbol* s) {
  if (!s || !s->name) { return; }
    switch(s->kind) {
    case SYMBOL_GLOBAL: fprintf(fp, "(kind: %s, name: %s, type: ", "global", s->name); break;
    case SYMBOL_LOCAL:  fprintf(fp, "(kind: %s, name: %s, type: ", "local", s->name);
    case SYMBOL_PARAM: fprintf(fp, "(kind: %s, name: %s, type: ", "parameter", s->name);
  }
  type_fprint(fp, s->type);
  if (s->which >= 0) { fprintf(fp, ", which: %d", s->which); }
  fprintf(fp, ")\n");
}

struct symbol* symbol_copy(struct symbol* s) {
  if (!s) { return NULL; }
  struct symbol* copy = NULL;
  copy = symbol_create(s->kind, type_copy(s->type), s->name);
  if (copy) { copy->which = s->which; }
  return copy;
}

/* generates the proper address/label for a symbol */
const char* symbol_codegen(struct symbol* s) {
  if (!s) { /* TODO: error message, null, failed to generate address */ return NULL; }
  if (s->address) { free(s->address); }
  switch (s->kind) {
    case SYMBOL_GLOBAL: strcpy(symbol_address, s->name); break;
    default: strcpy(symbol_address, "\0"); sprintf(symbol_address, "-%d(%%rbp)", (s->which + 1) * QUAD); break;
  }
  s->address = strdup(symbol_address);
  return s->address;
}

