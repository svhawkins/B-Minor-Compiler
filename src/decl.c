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
    default: break;
  }
  return buffer;
}

char* decl_codegen_strerror(decl_codegen_error_t kind) {
  switch(kind) {
    case DECL_NEGSIZE: strcpy(buffer, "ENEGSIZE"); break;
    case DECL_PADSIZE: strcpy(buffer, "WSIZE"); break;
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
    default:
    break;
  }
  fprintf(ERR_OUT, "\n\n");
  global_error_count++;
  return kind;
}

int decl_codegen_error_handle(decl_codegen_error_t kind, void* ctx1, void* ctx2) {
  bool is_fatal = false;
  switch(kind) {
    case DECL_NEGSIZE: /* negative size detected */
    is_fatal = true;
    fprintf(ERR_OUT, "ERROR %s (%d): ", decl_codegen_strerror(kind), kind);
    fprintf(ERR_OUT, "Negative size field detected in declaration. In declaration:\n");
    decl_fprint(ERR_OUT, ((struct decl*)ctx1), 0);
    fprintf(ERR_OUT, " with evaluated size: %ld", ((struct decl*)ctx1)->type->actual_size);
    break;
    case DECL_PADSIZE: /* size mismatch, using size determined from value field.*/
    fprintf(ERR_OUT, "WARNING %s (%d): ", decl_codegen_strerror(kind), kind);
    fprintf(ERR_OUT, "Declared size and list size mismatch. Declared size: ");
    expr_fprint(ERR_OUT, ((struct decl*)ctx1)->type->size);
    fprintf(ERR_OUT, " (%ld)", ((struct decl*)ctx1)->type->actual_size); 
    fprintf(ERR_OUT, "True array size is list size: %d.", *((int*)ctx2));
    break;
  }
  fprintf(ERR_OUT, "\n\n");
  global_error_count++;
  if (!is_fatal || is_test) { return kind; }
  exit(kind);
}

/* generates a decl expression. single expression. */
int decl_codegen_expr(Symbol_table* st, struct decl* d, struct expr* e)
{
    // get the resulting expression register
    // global decls don't print anything, locals don't put to stack until end

    switch(d->symbol->kind) {
      case SYMBOL_GLOBAL:
        /*
          .quad <value>
          .zero 8 # if no value
        */
        generate_expr = false;
        error_status = expr_codegen(st, e);
        if (e && e->string_literal) {
          fprintf(CODEGEN_OUT, "\t.quad %s\n", symbol_table_hidden_lookup(st->hidden_table, e->string_literal));
          register_scratch_free(e->reg);
        }
        else if (e) { fprintf(CODEGEN_OUT, "\t.quad %ld\n", e->literal_value); register_scratch_free(e->reg); }
        else { fprintf(CODEGEN_OUT, "\t.zero %d\n", QUAD); }
      break;
      case SYMBOL_LOCAL:
        /*
        MOVQ e->reg, symbol_codegen(e->symbol)

        if undefined, value is default 0.
        */
        generate_expr = true;
        error_status = expr_codegen(st, e);
        symbol_codegen(d->symbol); /* to keep regenerating the same thing, slightly altered which field */

        if (e) {
          fprintf(CODEGEN_OUT, "MOVQ %s, %s\n", register_scratch_name(e->reg), d->symbol->address);
          register_scratch_free(e->reg); // it got moved to the stack, the intermediary register is free now.
        } else {
          fprintf(CODEGEN_OUT, "MOVQ %s, %s\n", "$0", d->symbol->address);
        }
      break;
      case SYMBOL_PARAM:
        // ?????
      break;
    }
    d->symbol->which++;
    st->which_count->items[symbol_table_scope_level(st)]++;
    return error_status;
}

/* 
   generates array declarations
   return value is either -1 (error), 0 (no generation), 1 (successfully generated)
   returns a sum only in the 'intermediate' calls
 */
enum { DECL_ERROR = -1, DECL_NO_GEN = 0, DECL_SUCCESS = 1};


int decl_codegen_array(Symbol_table* st,
		       struct decl* d,
		       struct expr* e,
		       struct type* t,
		       int* count,
		       int* limit)
{
  // 'base' case, generating primitive expressions
  if (e && (e->kind != EXPR_INIT) && (e->kind != EXPR_COMMA)) {
   // generating w/o threshold or only within threshold
   if ((*limit < 0) || (*count < *limit)) { /* this short circuits, no need for limit >= 0 */
    error_status = decl_codegen_expr(st, d, e);
    *count += ((!error_status) ? 1 : 0);
    return (!error_status) ? DECL_SUCCESS : DECL_ERROR;
    } else { return DECL_NO_GEN; }
  }

  // 'traversal' case, gotta reach the children somehow
  if (e && (e->kind == EXPR_COMMA)) {
    int left_size = decl_codegen_array(st, d, e->left, t, count, limit);
    int right_size = decl_codegen_array(st, d, e->right, t, count, limit);
    return ((left_size != DECL_ERROR) && right_size != DECL_ERROR) ? left_size + right_size : DECL_ERROR;
    // FIXME: return count instead?
  }

  // 'entry' case: current expression (or type, or both) is an array
  if ((e && e->kind == EXPR_INIT) || (t->kind == TYPE_ARRAY))
  {
    bool passed_in = (*count >= 0 && *limit >= 0); // use parameters from previous calls, 'parent' arrays
    bool generated_array = false;
    
    // generate declared size. no expression actually generated. just need the consteval result.
    generate_expr = false;
    error_status = expr_codegen(st, t->size);
    if (t->size && (t->size->literal_value < 0)) {
      /* fatal error, negative size declared */
      error_status = decl_codegen_error_handle(DECL_NEGSIZE, d, NULL);
      return DECL_ERROR;
    }

    /* only generate the next array if fits within the current threshold */
    if ((!passed_in) || (passed_in && (*count < *limit))) {
      int child_limit = (t->size) ? t->size->literal_value : t->actual_size; // default -1
      int child_size = 0;
      int return_size = 0;

      // generate from the list itself iff there is an expression
      if (e) {
        return_size = decl_codegen_array(st, d, e->left, t->subtype, &child_size, &child_limit);
      }
      if (return_size == DECL_ERROR) { return DECL_ERROR; }

      // verify the value

      // apply any padding if less than threshold
      // over threshold handled in different recursion cases.
      uint64_t delta = (child_limit >= 0) ? (child_limit - return_size) : 0;
      if (delta > 0) {
        /* warning, doing additional padding . <-- quiet error? suppressable? */
        // FIXME: update error message to do valid memory access
        //error_status = decl_codegen_error_handle(DECL_PADSIZE, d, NULL);
      }
      for (uint64_t i = 0; i < delta; i++) { decl_codegen_expr(st, d, NULL); return_size += 1; }

      // FIXME: is this ever triggered?
      if ((child_limit >= 0) && (return_size != child_limit)) { return DECL_ERROR; }
      t->actual_size = return_size; // this is used for further arrays at the same depth level.
      *count += (passed_in) ? 1 : 0;
      generated_array = true;
    } else {
      /* emit warning about truncating current array in decl d? */
      generated_array = false;
    }
    return (generated_array) ? DECL_SUCCESS : DECL_NO_GEN;
  }
}

/*********************
 * 
 * 'EXTERNAL' FUNCTIONS
 * 
**********************/
struct decl* decl_create(char* name, struct type* type, struct expr* value, struct stmt* code, struct decl* next)
{
  struct decl* d = malloc(sizeof(struct decl));
  if (d) {
    d->name = (name != NULL) ? strdup(name) : NULL;
    d->type = type;
    d->value = value;
    d->code = code;
    d->next = next;
    d->symbol = NULL;
  }
  return d;
}


void decl_fprint(FILE* fp, struct decl* d, int indent) {
  if (!d) { return; }
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
    else { fprintf(fp, ";"); }
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

void name_destroy(char* name) { free(name); }
void decl_destroy(struct decl** d) {
  if (!d || !(*d)) { return; }
  type_destroy(&((*d)->type));
  stmt_destroy(&((*d)->code));
  expr_destroy(&((*d)->value));
  name_destroy((*d)->name);
  decl_destroy(&((*d)->next));
  free(*d);
  *d = NULL;
}


int decl_resolve(struct symbol_table* st, struct decl* d) {
  if (!st || !d) { return error_status; decl_error = NO_ERROR; }
  symbol_t kind = symbol_table_scope_level(st) > 1 ? SYMBOL_LOCAL : SYMBOL_GLOBAL;
  // does this declaration require constant expressions?
  is_const_expr = ((d->value && kind == SYMBOL_GLOBAL) || d->type->size);

  d->symbol = symbol_create(kind, type_copy(d->type), d->name);
  // look up symbol in table
  struct symbol* sym = symbol_table_scope_lookup_current(st, d->name);
  if (!sym) {
    // add symbol to the table
    symbol_table_scope_bind(st, d->name, d->symbol);
    if (d->type->size) { 
      error_status = expr_resolve(st, d->type->size);
      if (decl_error) { error_status = decl_error_handle(DECL_CONST, (void*)d, NULL); }
    }
    if (d->value) {
      error_status = expr_resolve(st, d->value);
      if (decl_error) { error_status = decl_error_handle(DECL_CONST, (void*)d, NULL); }
    }
    if (d->code || d->value) { d->symbol->defined = true; }
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
    symbol_table_scope_enter(st); // enter parameter (and forthcoming body) scope
    error_status = param_list_resolve(st, d->type->params); // so d->code won't have undefined references :)
    if (d->code) { error_status = stmt_resolve(st, d->code); }
    symbol_table_scope_exit(st); // exit parameter scope
  }
  error_status = decl_resolve(st, d->next);
  return error_status;
}

int decl_typecheck(struct symbol_table* st, struct decl* d) {
  if (!d) { return error_status; }

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

// generates code for declaration structures.
int decl_codegen(struct symbol_table* st, struct decl* d) {
  if (!d) { return error_status; }

  /* only generate hidden symbol TABLE if not done so already  */
  if (st->hidden_generated == false) { symbol_table_hidden_codegen(st->hidden_table); st->hidden_generated = true; }

  /* per declaration */
  switch (d->symbol->kind) {
    case SYMBOL_LOCAL:
      /* case SYMBOL_PARAM: ??? <-- TO DO */
      // assign the which count the count from previous scope
      d->symbol->which = (st->which_count->items[symbol_table_scope_level(st) - 1]);
    break;
    case SYMBOL_GLOBAL:
      // generate the declaration label
      if (!d->symbol->address) { symbol_codegen(d->symbol); }
      fprintf(CODEGEN_OUT, "%s:\n", d->symbol->address);
    break;
  }

 // generate the expression(s)/statement(s)
 int count = -1, limit = -1, old_which = -1;
 switch (d->type->kind)
 {
  case TYPE_FUNCTION: /* TODO */
  break;
  case TYPE_ARRAY: /* multiple decl_codegen_expr, size checking */
    old_which = (d->symbol->kind == SYMBOL_LOCAL) ? d->symbol->which : 0;
    error_status = decl_codegen_array(st, d, d->value, d->type, &count, &limit);
    if (error_status == DECL_ERROR) {
      // somehow handle here?? generic fatal error???
      return DECL_ERROR;
    }
    d->symbol->which = old_which; // TODO: should i get rid of this??
  break;
  default: /* primitive type */
    decl_codegen_expr(st, d, d->value);
  break;
 }
 error_status = decl_codegen(st, d->next);
 return error_status;
}
