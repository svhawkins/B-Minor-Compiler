#include <stdio.h>
#include <stdlib.h>
#include "decl.h"

extern void print_indent(FILE* fp, int indent);
const char* SPACE = "  ";
char buffer[16]; // for error strings
char* decl_strerror(decl_error_t kind) {
  switch(kind) {
    case DECL_NULL: strcpy(buffer, "ENULL"); break;
    case DECL_NINT: strcpy(buffer, "ENINT"); break;
    case DECL_CONST: strcpy(buffer, "ECONST"); break;
  }
  return buffer;
}

int decl_error_handle(decl_error_t kind, void* ctx1, void* ctx2) {
  fprintf(ERR_OUT, "ERROR %s (%d): ", decl_strerror(kind), kind);
  switch (kind) {
    case DECL_NULL: /* array size field is null when it should have a contained expression */
    fprintf(ERR_OUT, "Indeterminate array size. Arrays must have a size expression or an associated value.\n");
    fprintf(ERR_OUT, "Of symbol: "); symbol_fprint(ERR_OUT, (struct symbol*)ctx1);
    fprintf(ERR_OUT, "With value: "); if (ctx2) { expr_fprint(ERR_OUT, (struct expr*)ctx2); } else { fprintf(ERR_OUT, "[null]"); }
    break;
    case DECL_NINT: /* array size field should evaluate to INTEGER and nothing else */
    fprintf(ERR_OUT, "Array size is non-integer. Array sizes must have expressions to evaulate to INTEGER.\n");
    fprintf(ERR_OUT, "Of symbol: "); symbol_fprint(ERR_OUT, (struct symbol*)ctx1);
    fprintf(ERR_OUT, "With array size expression type: "); type_fprint(ERR_OUT, (struct type*)ctx2);
    break;
    case DECL_CONST: /* array declaration sizes and global variable declarations must be constant */
    fprintf(ERR_OUT, "Array size or global variable value expression is non-constant.\n");
    fprintf(ERR_OUT, "Array size expressions or global variable expressions cannot contain variables.\n");
    fprintf(ERR_OUT, "in declaration: "); decl_fprint(ERR_OUT, (struct decl*)ctx1, 0);
    break;
  }
  fprintf(ERR_OUT, "\n\n");
  global_error_count++;
  return kind;
}


struct decl* decl_create(char* name, struct type* type, struct expr* value, struct stmt* code, struct decl* next)
{
  struct decl* d = malloc(sizeof(struct decl));
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
  if (!st || !d) return error_status; decl_error = NO_ERROR;
  symbol_t kind = symbol_table_scope_level(st) > 1 ? SYMBOL_LOCAL : SYMBOL_GLOBAL;
  // does this declaration require constant expressions?
  is_const_expr = ((d->value && kind == SYMBOL_GLOBAL) || d->type->size);

  d->symbol = symbol_create(kind, type_copy(d->type), strdup(d->name));
  // look up symbol in table
  struct symbol* sym = symbol_table_scope_lookup_current(st, d->name);
  if (!sym) {
    // add symbol to the table
    if (d->type->size) { 
      error_status = expr_resolve(st, d->type->size);
      if (decl_error) { error_status = decl_error_handle(DECL_CONST, (void*)d, NULL); }
    }
    if (d->value) {
      error_status = expr_resolve(st, d->value);
      if (decl_error) { error_status = decl_error_handle(DECL_CONST, (void*)d, NULL); }
    }
    if (d->code || d->value) { d->symbol->defined = true; }
    symbol_table_scope_bind(st, d->name, d->symbol);
  } else {
    // symbol is already being used
    if (d->type->kind == TYPE_FUNCTION && !sym->defined && d->code) {
      sym->defined = true;
      if (!param_list_equals(sym->type->params, d->type->params)) {
        error_status = symbol_table_error_handle(SYM_PARAM, (void*)sym->type, (void*)d->type);
      }
    }
    else { error_status = symbol_table_error_handle(SYM_REDEF, (void*)d->symbol, (void*)sym); }
    symbol_destroy(&d->symbol);
  }
  // resolve the function parameters and body (if valid declaration)
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
  if (!d) return error_status;

  // corresponding value expressions must evaluate to same type declared
  if (d->value) {
    struct type* t = expr_typecheck(st, d->value);
    // auto declarations update their type to value type.
    if (d->type->kind == TYPE_AUTO) {
	    type_destroy(&d->type); d->type = type_copy(t);
	    type_destroy(&d->symbol->type); d->symbol->type = type_copy(t);
    }
    if (d->symbol && !type_equals(t, d->symbol->type)) {
        error_status = symbol_table_error_handle(SYM_TYPE, (void*)d->symbol, (void*)t);
    }
    type_destroy(&t);
  }
  
  // arrays cannot have both NULL size and value
  if (d->type->kind == TYPE_ARRAY) {
    if (!d->type->size && !d->value) { error_status = decl_error_handle(DECL_NULL, (void*)d->symbol, (void*)d->value); }
    else if (d->type->size) {
        struct type* size_type = expr_typecheck(st, d->type->size);
        if (size_type->kind != TYPE_INTEGER) { error_status = decl_error_handle(DECL_NINT, (void*)d->symbol, (void*)size_type); }
        type_destroy(&size_type);
    }
  }
  
  // defined functions must let their return value match declared return type
  if (d->type->kind == TYPE_FUNCTION && d->symbol) {
   symbol_table_scope_enter(st);
   if (d->code) {
      struct type* ret_type = NULL;
      error_status = stmt_typecheck(st, d->code, &ret_type);
      if (!ret_type) ret_type = type_create(TYPE_VOID, NULL, NULL, NULL);
      if (!type_equals(d->type->subtype, ret_type)) {
        error_status = symbol_table_error_handle(SYM_TYPE, (void*)d->symbol, (void*)ret_type);
      }
      type_destroy(&ret_type);
    } symbol_table_scope_exit(st);
  }
  
  error_status = decl_typecheck(st, d->next);
  return error_status;
}
