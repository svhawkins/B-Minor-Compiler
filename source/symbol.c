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
