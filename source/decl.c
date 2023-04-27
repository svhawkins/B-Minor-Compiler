#include <stdio.h>
#include <stdlib.h>
#include "decl.h"

extern void print_indent(FILE* fp, int indent);
const char* SPACE = "  ";
struct decl* decl_create(char* name, struct type* type, struct expr* value, struct stmt* code, struct decl* next)
{
  struct decl* d = malloc(sizeof(*d));
  if (d) {
    d->name = name;
    d->type = type;
    d->value = value;
    d->code = code;
    d->next = next;
    d->symbol = NULL;
  }
  return d;
}


void decl_fprint(FILE* fp, struct decl* d, int indent) {
  if (!d) return;
  print_indent(fp, indent);

  // name : type
  fprintf(fp, "%s: ", d->name); type_fprint(fp, d->type);

  // init or uninit?
  switch(d->type->kind) {
  case TYPE_FUNCTION:
    if (d->code) {
      fprintf(fp, " = ");
      stmt_fprint(fp, d->code, indent);
      print_indent(fp, indent); fprintf(fp, "\n");
    }
    else fprintf(fp, ";");
    break;
  default:
    if (d->value) {
      fprintf(fp, " = ");
      expr_fprint(fp, d->value);
    }
    fprintf(fp, ";");
    break;
  }
  if (d->next) { fprintf(fp, "\n"); }
  decl_fprint(fp, d->next, indent);
}

void decl_print(struct decl* d, int indent) { decl_fprint(stdout, d, indent); }

void decl_destroy(struct decl** d) {
  if (!(*d)) return;
  free((*d)->name);
  type_destroy(&((*d)->type));
  expr_destroy(&((*d)->value));
  stmt_destroy(&((*d)->code));
  //symbol_destroy(&((*d)->symbol));
  decl_destroy(&((*d)->next));
  free(*d); *d = NULL;
}


void decl_resolve(struct symbol_table* st, struct decl* d) {
  if (!st || !d) return;
  symbol_t kind = symbol_table_scope_level(st) > 1 ? SYMBOL_LOCAL : SYMBOL_GLOBAL;
  d->symbol = symbol_create(kind, type_copy(d->type), strdup(d->name));
  expr_resolve(st, d->value); // names in the expression should already be defined/declared.

  // TO DO: error messages
  symbol_table_scope_bind(st, d->name, d->symbol);
  if (d->code) {
    symbol_table_scope_enter(st);
    param_list_resolve(st, d->type->params); // so d->code won't have undefined references :)
    stmt_resolve(st, d->code);
    symbol_table_scope_exit(st);
  }
  decl_resolve(st, d->next);
}

void decl_typecheck(struct symbol_table* st, struct decl* d) {
  if (!d) return;
  if (!type_equals(d->type, d->symbol->type)) { /* error message */ }
  if (d->value) {
    struct type* t = expr_typecheck(st, d->value);
    if (!type_equals(t, d->symbol->type)) { /* TO DO: error message */ }
  }
  if (d->code) {
  //if (!type_equals(d->type, d->symbol->type)) { /* error message */ }
  //if (!param_list_equals(d->type->params, d->symbol->type->params)) { /* error message */ }
    stmt_typecheck(st, d->code); // TO DO: maybe include type/its symbol type as a parameter for return statement typechecking.
  }
}
