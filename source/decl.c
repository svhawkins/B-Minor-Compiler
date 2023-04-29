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
  decl_destroy(&((*d)->next));
  free(*d); *d = NULL;
}


int decl_resolve(struct symbol_table* st, struct decl* d) {
  int error_status = 0;
  if (!st || !d) return error_status;
  symbol_t kind = symbol_table_scope_level(st) > 1 ? SYMBOL_LOCAL : SYMBOL_GLOBAL;
  d->symbol = symbol_create(kind, type_copy(d->type), strdup(d->name));
  error_status = expr_resolve(st, d->value); // names in the expression should already be defined/declared by now.

  struct symbol* sym = symbol_table_scope_lookup_current(st, d->name);
  if (sym) error_status = symbol_table_error_handle(SYM_REDEF, (void*)d->symbol, (void*)sym);
  else symbol_table_scope_bind(st, d->name, d->symbol);

  if (d->type->kind == TYPE_FUNCTION) {
    symbol_table_scope_enter(st);
    error_status = param_list_resolve(st, d->type->params); // so d->code won't have undefined references :)
    if (d->code) { error_status = stmt_resolve(st, d->code); }
    symbol_table_scope_exit(st);
  }
  error_status = decl_resolve(st, d->next);
  return error_status;
}

int decl_typecheck(struct symbol_table* st, struct decl* d) {
  int error_status = 0;
  if (!d) return error_status;
  if (d->value) {
    struct type* t = expr_typecheck(st, d->value);
    // auto declarations update their type to value type.
    if (d->type->kind == TYPE_AUTO) {
	type_destroy(&d->type); d->type = type_copy(t);
	type_destroy(&d->symbol->type); d->symbol->type = type_copy(t);
    }
    if (!type_equals(t, d->symbol->type)) {
    error_status = symbol_table_error_handle(SYM_TYPE, (void*)d->symbol, (void*)t);
    } type_destroy(&t);
  }
  if (d->code) {
    struct type* ret_type = NULL;
    if (!param_list_equals(d->type->params, d->symbol->type->params)) {
      error_status = symbol_table_error_handle(SYM_PARAM, (void*)d->symbol->type, (void*)d->type);
    }
    error_status = stmt_typecheck(st, d->code, &ret_type);
    if (!type_equals(d->type->subtype, ret_type)) { error_status = symbol_table_error_handle(SYM_TYPE, (void*)d->symbol, (void*)ret_type); }
  }
  return decl_typecheck(st, d->next);
}
