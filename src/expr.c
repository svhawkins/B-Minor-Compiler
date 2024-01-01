#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "expr.h"

enum { UNDERFLOW = false, OVERFLOW = true };
inline bool flow_check(expr_t kind, int64_t result, int64_t left, int64_t right, bool is_overflow) {
  bool overflow = false, underflow = false;
  //printf("result: %ld\tleft: %ld\tright: %ld\n", result, left, right);
  switch(kind) {
    case EXPR_ADD:
      overflow = (left >= 0 && right >= 0 && result < 0);
      underflow = (left < 0 && right < 0 && result >= 0);
    break;
    case EXPR_INC:
        overflow = (left >= 0 && result < 0);
        underflow = (left < 0 && result >= 0);
    break;
    case EXPR_SUB:
      overflow = (left >= 0 && right < 0 && result < 0); /* +L --R --> +L + R*/
      underflow = (left < 0 && right >= 0 && result >= 0); /* -L - +R --> -L -R */
    break;
    case EXPR_DEC:
      overflow = (left >= 0 && result < 0);
      underflow = (left < 0 && result >= 0);
    break;
    case EXPR_MULT:
      overflow = (left < 0 && right < 0 && result < 0) || (left >= 0 && right >= 0 && result < 0); /* -L * -R -> +, +L * +R --> +*/
      underflow = (left < 0 && right >= 0 && result >= 0) || (left >= 0 && right < 0 && result >= 0);
    break;
    case EXPR_DIV:
    case EXPR_MOD:
      underflow = false;
      overflow = (left == INT64_MIN && right == - 1); /* overflow only occurs with negative operands */
    break;
    case EXPR_EXP: // TO DO /* overflow: NaN, underflow: unnormalized*/
    default: break; /* do nothing */
  }
  return (is_overflow) ? overflow : underflow;
}

// it is here as not realloc every time
char buffer[16];
// handles error messages
char* expr_strerror(type_error_t kind) {
  switch(kind) {
    case MATH: strcpy(buffer, "EMATH"); break;
    case RELATE: strcpy(buffer, "ERELATE"); break;
    case LOGIC: strcpy(buffer, "ELOGIC"); break;
    case EQUAL: strcpy(buffer, "EEQUAL"); break;
    case ASSIGN: strcpy(buffer,"EASSIGN"); break;
    case LVAL: strcpy(buffer, "ELVAL"); break;
    case IMMUTABLE: strcpy(buffer, "EIMMUTABLE"); break;
    case SUBSCRIPT: strcpy(buffer, "ESUBSCRIPT"); break;
    case FCALL: strcpy(buffer, "EFCALL"); break;
    case PARAM: strcpy(buffer, "EPARAM");
  }
  return buffer;
}
int expr_type_error_handle(type_error_t kind, void* ctx1, void* type_ctx1, void* ctx2, void* type_ctx2) {
  fprintf(ERR_OUT, "ERROR %s (%d):\n", expr_strerror(kind), kind);
  switch (kind) {
    case MATH: /* operands must be integers */
    case RELATE:
      fprintf(ERR_OUT, "Invalid operand type(s).\n");
      fprintf(ERR_OUT, "All operand(s) must be of type INTEGER.\n");
      fprintf(ERR_OUT, "Operand 1: "); expr_fprint(ERR_OUT, (struct expr*)ctx1);
      fprintf(ERR_OUT, " with type "); type_fprint(ERR_OUT, (struct type*)type_ctx1);
      if (ctx2) {
        fprintf(ERR_OUT, "\nOperand 2: "); expr_fprint(ERR_OUT, (struct expr*)ctx2);
        fprintf(ERR_OUT, " with type "); type_fprint(ERR_OUT, (struct type*)type_ctx2);
      }
    break;
    case LOGIC: /* operands must be booleans */
      fprintf(ERR_OUT, "Invalid operand type(s).\n");
      fprintf(ERR_OUT, "All operand(s) must be of type BOOLEAN.\n");
      fprintf(ERR_OUT, "Operand 1: "); expr_fprint(ERR_OUT, (struct expr*)ctx1);
      fprintf(ERR_OUT, " with type "); type_fprint(ERR_OUT, (struct type*)type_ctx1);
      if (ctx2) {
        fprintf(ERR_OUT, "\nOperand 2: "); expr_fprint(ERR_OUT, (struct expr*)ctx2);
        fprintf(ERR_OUT, " with type "); type_fprint(ERR_OUT, (struct type*)type_ctx2);
      }
    break;
    case EQUAL: /* operands must be the same, cannot be function/array */
      fprintf(ERR_OUT, "Invalid operand type(s).\n");
      fprintf(ERR_OUT, "Operand(s) must be the same type of either INTEGER, CHAR, BOOLEAN, or STRING.\n");
      fprintf(ERR_OUT, "Operand 1: "); expr_fprint(ERR_OUT, (struct expr*)ctx1);
      fprintf(ERR_OUT, " with type "); type_fprint(ERR_OUT, (struct type*)type_ctx1);
      if (ctx2) {
        fprintf(ERR_OUT, "\nOperand 2: "); expr_fprint(ERR_OUT, (struct expr*)ctx2);
        fprintf(ERR_OUT, " with type "); type_fprint(ERR_OUT, (struct type*)type_ctx2);
      }
    break;
    case ASSIGN: /* operands must be the same type*/
      fprintf(ERR_OUT, "Invalid operand type(s).\n");
      fprintf(ERR_OUT, "Operand(s) must be of the same type.\n");
      fprintf(ERR_OUT, "Operand 1: "); expr_fprint(ERR_OUT, (struct expr*)ctx1);
      fprintf(ERR_OUT, " with type "); type_fprint(ERR_OUT, (struct type*)type_ctx1);
      if (ctx2) {
        fprintf(ERR_OUT, "\nOperand 2: "); expr_fprint(ERR_OUT, (struct expr*)ctx2);
        fprintf(ERR_OUT, " with type "); type_fprint(ERR_OUT, (struct type*)type_ctx2);
      }
    break;
    case LVAL: /* left operand cannot be a literal */
      fprintf(ERR_OUT, "Invalid value of operand(s).\n");
      fprintf(ERR_OUT, "Operand 1 must be NOT be a literal value.\n");
      fprintf(ERR_OUT, "Operand 1: "); expr_fprint(ERR_OUT, (struct expr*)ctx1);
    break;
    case IMMUTABLE: /* LHS of assignment operation cannot be a string */
      fprintf(ERR_OUT, "Invalid LHS of assignment\n");
      fprintf(ERR_OUT, "Operand 1 must be of mutable type.\n");
      fprintf(ERR_OUT, "Operand 1: "); expr_fprint(ERR_OUT, (struct expr*)ctx1);
      fprintf(ERR_OUT, " with type "); type_fprint(ERR_OUT, (struct type*)type_ctx1);
    break;
    case INIT: /* operands within list must be the same */
      fprintf(ERR_OUT, "Invalid operand type(s).\n");
      fprintf(ERR_OUT, "All Operand(s) within initializer list must be of the same type.\n");
      fprintf(ERR_OUT, "Operand 1: "); expr_fprint(ERR_OUT, (struct expr*)ctx1);
      fprintf(ERR_OUT, " with type "); type_fprint(ERR_OUT, (struct type*)type_ctx1);
      if (ctx2) {
        fprintf(ERR_OUT, "\nOperand 2: "); expr_fprint(ERR_OUT, (struct expr*)ctx2);
        fprintf(ERR_OUT, " with type "); type_fprint(ERR_OUT, (struct type*)type_ctx2);
      }
    break;
    case SUBSCRIPT: /* left must be array, right must be integer */
      fprintf(ERR_OUT, "Invalid operand type(s).\n");
      fprintf(ERR_OUT, "Operand 1 must be of type ARRAY and operand 2 of type INTEGER.\n");
      fprintf(ERR_OUT, "Operand 1: "); expr_fprint(ERR_OUT, (struct expr*)ctx1);
      fprintf(ERR_OUT, " with type "); type_fprint(ERR_OUT, (struct type*)type_ctx1);
      if (ctx2) {
        fprintf(ERR_OUT, "\nOperand 2: "); expr_fprint(ERR_OUT, (struct expr*)ctx2);
        fprintf(ERR_OUT, " with type "); type_fprint(ERR_OUT, (struct type*)type_ctx2);
      }
    break;
    case FCALL: /* left must be a function */
      fprintf(ERR_OUT, "Invalid operand type(s).\n");
      fprintf(ERR_OUT, "Operand 1 must be of type FUNCTION.\n");
      fprintf(ERR_OUT, "Operand 1: "); expr_fprint(ERR_OUT, (struct expr*)ctx1);
      fprintf(ERR_OUT, " with type "); type_fprint(ERR_OUT, (struct type*)type_ctx1);
    break;
    case PARAM: /* argument types must match with parameter list types */
      fprintf(ERR_OUT, "Invalid argument type(s).\n");
      fprintf(ERR_OUT, "Types of expressions with argument list must match declared parameter types");
      fprintf(ERR_OUT, "\nOf symbol: "); symbol_fprint(ERR_OUT, (struct symbol*)ctx1);
      fprintf(ERR_OUT, "\nWith argument(s): "); expr_fprint(ERR_OUT, (struct expr*)type_ctx1);
    break;
  }
  fprintf(ERR_OUT, "\n\n");
  global_error_count++;
  return kind;
}

char* expr_codegen_strerror(codegen_error_t kind) {
  switch(kind) {
    case ERR_OVERFLOW: strcpy(buffer, "EOVERFLOW"); break;
    case ERR_UNDERFLOW: strcpy(buffer, "EUNDERFLOW"); break;
    case EXPR_BYZERO: strcpy(buffer, "EBYZERO"); break;
  }
  return buffer;
}

int expr_codegen_error_handle(codegen_error_t kind, struct expr* e) {
  fprintf(ERR_OUT, "ERROR %s (%d):\n", expr_codegen_strerror(kind), kind);
  switch(kind) {
    case ERR_OVERFLOW: /* operation detected integer overflow */
      fprintf(ERR_OUT, "WARNING: integer overflow detected in expression: ");
      break;
    case ERR_UNDERFLOW: /* operation detected integer underflow*/
      fprintf(ERR_OUT, "WARNING: integer underflow detected in expression: ");
      break;
    case EXPR_BYZERO: /* modulus or division by zero detected */
      fprintf(ERR_OUT, "WARNING: division or modulus by zero attempted in expression: ");
      break;
  }
  expr_fprint(ERR_OUT, e);
  fprintf(ERR_OUT, "\n\n");
  global_error_count++;
  return kind;
}

// helper functions
bool expr_is_primitive(expr_t kind) { return (kind >= EXPR_NAME); }
bool expr_is_unary(expr_t kind) { return (kind >= EXPR_INC && kind <= EXPR_NOT); }
bool expr_is_binary(expr_t kind) { return (kind >= EXPR_EXP && kind <= EXPR_COMMA); }
// excludes subscript [] and fcall () since those have right subtree within operator.
bool expr_is_wrap(expr_t kind) { return (kind >= EXPR_SUBSCRIPT && kind <= EXPR_INIT); }


// adds parentheses to child expression if nonprimitive
void expr_child_fprint(FILE* fp, struct expr* c) {
  if (!c) return;
  if (expr_is_primitive(c->kind)) expr_fprint(fp, c);
  else { fprintf(fp, "("); expr_fprint(fp, c); fprintf(fp, ")"); }
}

// compares a parameter list to its argument list in regard to types
bool fcall_compare(struct symbol_table* st, struct param_list* p, struct expr* e) {
  if (!p || !e) { return (!p && !e); } // null cases
  // comma expression types indicate an expression list
  bool is_expr_list = (e->kind == EXPR_COMMA);
  struct type* expr_type = (is_expr_list) ? expr_typecheck(st, e->left) : expr_typecheck(st, e);
  bool is_type_equal = type_equals(p->type, expr_type); type_destroy(&expr_type);
  return is_type_equal && ((is_expr_list) ? fcall_compare(st, p->next, e->right) : 1);
}

// typechecks an initialization list
// seperate from EXPR_COMMA, since that can be any type
// initialization lists must have elements all be of the same type.
struct type* expr_typecheck_init(struct symbol_table* st, struct expr* e) {
    // compare the expression list within
    bool is_expr_list = (e->kind == EXPR_COMMA);
    struct type* lt = (is_expr_list) ? expr_typecheck(st, e->left) : expr_typecheck(st, e);
    struct type* rt = (is_expr_list) ? expr_typecheck(st, e->right) : NULL;
    if (is_expr_list && !type_equals(lt, rt)) {
      error_status = expr_type_error_handle(INIT, (void*)e->left, (void*)lt, (void*)e->right, (void*)rt);
    }
    type_destroy(&rt);
    return lt;
}

struct expr* expr_create(expr_t kind, struct expr* left, struct expr* right )
{
  struct expr* e = malloc(sizeof(struct expr));
  if (e) {
    e->kind = kind;
    e->left = left;
    e->right = right;

    // non-leaf expressions recieve default values
    e->name = NULL;
    e->literal_value = 0;
    e->string_literal = NULL;
    e->symbol = NULL;
    e->reg = -1;
  }
  return e;
}


// delegated 'constructors' haha
struct expr* expr_create_name(const char* n)
{
  struct expr* e = expr_create(EXPR_NAME, NULL, NULL);
  if (e) { e->name = n; }
  return e;
}

struct expr* expr_create_integer_literal(int64_t c)
{
  struct expr* e = expr_create(EXPR_INT, NULL, NULL);
  if (e) { e->literal_value = c; }
  return e;
}

struct expr* expr_create_boolean_literal(int c)
{
  struct expr* e = expr_create(EXPR_BOOL, NULL, NULL);
  if (e) { e->literal_value = c; }
  return e;
}

struct expr* expr_create_char_literal(char c)
{
  struct expr* e = expr_create(EXPR_CH, NULL, NULL);
  if (e) { e->literal_value = c; }
  return e;
}

struct expr* expr_create_string_literal(const char* str)
{
  struct expr* e = expr_create(EXPR_STR, NULL, NULL);
  if (e) { e->string_literal = str; }
  return e;
}

void expr_fprint(FILE* fp, struct expr* e) {
  if (!e) return;
  switch(e->kind) {
    // primitives
    case EXPR_NAME: fprintf(fp, "%s", e->name); break;
    case EXPR_CH: fprintf(fp, "'%c'", (int)e->literal_value); break;
    case EXPR_BOOL: fprintf(fp, "%s", (e->literal_value) ? "true" : "false"); break;
    case EXPR_STR: fprintf(fp, "\"%s\"", e->string_literal); break;
    case EXPR_INT: fprintf(fp, "%ld", e->literal_value); break;

    // operators
    // unary operators
    case EXPR_INC: expr_child_fprint(fp, e->left); fprintf(fp, "++"); break;
    case EXPR_DEC: expr_child_fprint(fp, e->left); fprintf(fp, "--"); break;
    case EXPR_POS: expr_fprint(fp, e->left); break; // +45 is just 45.
    case EXPR_NEG: fprintf(fp, "-"); expr_child_fprint(fp, e->left); break;
    case EXPR_NOT: fprintf(fp, "!"); expr_child_fprint(fp, e->left); break;

    // binary operators
    case EXPR_EXP: expr_child_fprint(fp, e->left); fprintf(fp, " ^ "); expr_child_fprint(fp, e->right); break;
    case EXPR_MULT: expr_child_fprint(fp, e->left); fprintf(fp, " * "); expr_child_fprint(fp, e->right); break;
    case EXPR_DIV: expr_child_fprint(fp, e->left); fprintf(fp, " / "); expr_child_fprint(fp, e->right); break;
    case EXPR_MOD: expr_child_fprint(fp, e->left); fprintf(fp, " %% "); expr_child_fprint(fp, e->right); break;
    case EXPR_ADD: expr_child_fprint(fp, e->left); fprintf(fp, " + "); expr_child_fprint(fp, e->right); break;
    case EXPR_SUB: expr_child_fprint(fp, e->left); fprintf(fp, " - "); expr_child_fprint(fp, e->right); break;
    case EXPR_LEQ: expr_child_fprint(fp, e->left); fprintf(fp, " <= "); expr_child_fprint(fp, e->right); break;
    case EXPR_LESS: expr_child_fprint(fp, e->left); fprintf(fp, " < "); expr_child_fprint(fp, e->right); break;
    case EXPR_GEQ: expr_child_fprint(fp, e->left); fprintf(fp, " >= "); expr_child_fprint(fp, e->right); break;
    case EXPR_GREAT: expr_child_fprint(fp, e->left); fprintf(fp, " > "); expr_child_fprint(fp, e->right); break;
    case EXPR_EQ: expr_child_fprint(fp, e->left); fprintf(fp, " == "); expr_child_fprint(fp, e->right); break;
    case EXPR_NEQ: expr_child_fprint(fp, e->left); fprintf(fp, " != "); expr_child_fprint(fp, e->right); break;
    case EXPR_AND: expr_child_fprint(fp, e->left); fprintf(fp, " && "); expr_child_fprint(fp, e->right); break;
    case EXPR_OR: expr_child_fprint(fp, e->left); fprintf(fp, " || "); expr_child_fprint(fp, e->right); break;

    // FOR NOW NO PAREN
    case EXPR_ASSIGN: expr_fprint(fp, e->left); fprintf(fp, " = "); expr_fprint(fp, e->right); break;
    case EXPR_COMMA: expr_fprint(fp, e->left); fprintf(fp, ", "); expr_fprint(fp, e->right); break;

    // wrap operators
    case EXPR_SUBSCRIPT: expr_fprint(fp, e->left); fprintf(fp, "["); expr_fprint(fp, e->right); fprintf(fp, "]"); break;
    case EXPR_FCALL: expr_fprint(fp, e->left); fprintf(fp, "("); expr_fprint(fp, e->right); fprintf(fp, ")"); break;
    case EXPR_INIT: fprintf(fp, "{"); expr_fprint(fp, e->left); fprintf(fp, "}"); break;
  }
}

void expr_print(struct expr* e) { expr_fprint(stdout, e); }

void expr_destroy(struct expr** e) {
  if (!e || !(*e)) { return; }
  expr_destroy(&((*e)->left));
  expr_destroy(&((*e)->right));

  // free the pointers
  if ((*e)->kind == EXPR_NAME) { free((void*)(*e)->name); }

  // since string literals are part of the symbol table
  //if ((*e)->string_literal) { free((void*)(*e)->string_literal); }
  if ((*e)->symbol) { (*e)->symbol = NULL; }

  // free the register
  if ((*e)->reg > 0) { register_scratch_free((*e)->reg); }
  free(*e); *e = NULL;
}

struct expr* expr_copy(struct expr* e) {
  if (!e) return NULL;
  struct expr* copy = malloc(sizeof(struct expr));
  if (copy) {
    copy->kind = e->kind;
    switch(e->kind) {
    case EXPR_NAME:
      copy->name = strdup(e->name);
      copy->symbol = symbol_copy(e->symbol);
      break;
    case EXPR_INT: case EXPR_BOOL: case EXPR_CH: copy->literal_value = e->literal_value; break;
    case EXPR_STR: copy->string_literal = strdup(e->string_literal); break;
    default:
      copy->left = expr_copy(e->left);
      copy->right = expr_copy(e->right);
    break;
    }
  }
  return copy;
}

int expr_resolve(struct symbol_table* st, struct expr* e) {
  if (!st || !e) { return 0; }
  int error_status = 0;
  char* found_label = NULL;
  switch(e->kind) {
  case EXPR_NAME:
    // constant expressions cannot contain names
    if (is_const_expr) { error_status = decl_error = DECL_CONST; }

    // check to see if the symbol has already been declared
    e->symbol = symbol_table_scope_lookup(st, e->name);
    if (!e->symbol) {
      error_status = symbol_table_error_handle(SYM_UNDEF, (void*)st, (void*)e);
    }
    break;
  case EXPR_STR:
    // store the string literal symbol as a hidden symbol in the symbol table
    found_label = symbol_table_hidden_lookup(st->hidden_table, e->string_literal);
    if (!found_label) {
      found_label = label_name(label_create());
      symbol_table_hidden_bind(st->hidden_table, (const char*)e->string_literal, (const char*)strdup(found_label));
    }
    break;
  default:
    error_status = expr_resolve(st, e->left);
    error_status = expr_resolve(st, e->right);
  }
  return error_status;
}

type_t invalid_type(type_t kind) { return (kind == TYPE_VOID || kind == TYPE_ARRAY || kind == TYPE_FUNCTION); }
struct type* expr_typecheck(struct symbol_table* st, struct expr* e) {
  if (!e) return NULL;
  struct type* left_expr_type = expr_typecheck(st, e->left);
  struct type* right_expr_type = expr_typecheck(st, e->right);
  struct type* result = NULL; struct symbol* s = NULL;
  switch (e->kind) {
    // primitive expression kinds
    case EXPR_CH:
     result = type_create(TYPE_CHARACTER, NULL, NULL, NULL);
     break;
    case EXPR_BOOL:
     result = type_create(TYPE_BOOLEAN, NULL, NULL, NULL);
     break;
    case EXPR_STR:
     result = type_create(TYPE_STRING, NULL, NULL, NULL);
     break;
    case EXPR_INT:
     result = type_create(TYPE_INTEGER, NULL, NULL, NULL);
     break;
    case EXPR_NAME:
     s = symbol_table_scope_lookup(st, e->name);
     result = type_copy(s->type);
     break;

    // equality operators
    case EXPR_EQ:
    case EXPR_NEQ:
      if (invalid_type(left_expr_type->kind) || invalid_type(right_expr_type->kind) || !type_equals(left_expr_type, right_expr_type)) {
        error_status = expr_type_error_handle(EQUAL, (void*)e->left, (void*)left_expr_type, (void*)e->right, (void*)right_expr_type);
        result = type_copy(left_expr_type);
      }
      else { result = type_create(TYPE_BOOLEAN, NULL, NULL, NULL); }
      break;

    // relational operators
    case EXPR_GREAT:
    case EXPR_GEQ:
    case EXPR_LESS:
    case EXPR_LEQ:
      if (!type_equals(left_expr_type, right_expr_type) || left_expr_type->kind != TYPE_INTEGER || right_expr_type->kind != TYPE_INTEGER) {
        error_status = expr_type_error_handle(RELATE, (void*)e->left, (void*)left_expr_type, (void*)e->right, (void*)right_expr_type);
        result = type_copy(left_expr_type);
      }
      else result = type_create(TYPE_BOOLEAN, NULL, NULL, NULL);
      break;

    // logical operators
    case EXPR_AND:
    case EXPR_OR:
    case EXPR_NOT:
      if (left_expr_type->kind != TYPE_BOOLEAN ||
	       (e->right && (right_expr_type->kind != TYPE_BOOLEAN || !type_equals(left_expr_type, right_expr_type)))) {
         error_status = expr_type_error_handle(LOGIC, (void*)e->left, (void*)left_expr_type, (void*)e->right, (void*)right_expr_type);
         result = type_copy(left_expr_type);
      }
      else result = type_create(TYPE_BOOLEAN, NULL, NULL, NULL);
      break;

    // arithmetic operators
    case EXPR_POS:
    case EXPR_NEG:
    case EXPR_INC:
    case EXPR_DEC:
    case EXPR_ADD:
    case EXPR_SUB:
    case EXPR_MULT:
    case EXPR_DIV:
    case EXPR_MOD:
    case EXPR_EXP:
      if (left_expr_type->kind != TYPE_INTEGER ||
	       (e->right && (right_expr_type->kind != TYPE_INTEGER || !type_equals(left_expr_type, right_expr_type)))) {
	       error_status = expr_type_error_handle(MATH, (void*)e->left, (void*)left_expr_type, (void*)e->right, (void*)right_expr_type);
         result = type_copy(left_expr_type);
      }
      else result = type_create(TYPE_INTEGER, NULL, NULL, NULL);
      break;

    // subscript
    case EXPR_SUBSCRIPT:
      if (left_expr_type->kind != TYPE_ARRAY || right_expr_type->kind != TYPE_INTEGER) {
         error_status = expr_type_error_handle(SUBSCRIPT, (void*)e->left, (void*)left_expr_type, (void*)e->right, (void*)right_expr_type);
         result = type_copy(left_expr_type);
      }
      else result = type_copy(left_expr_type->subtype);
      break;

    // function call
    case EXPR_FCALL:
      if (left_expr_type->kind != TYPE_FUNCTION) {
       error_status = expr_type_error_handle(FCALL, (void*)e->left, (void*)left_expr_type, (void*)e->right, (void*)right_expr_type);
       result = type_copy(left_expr_type); break;
      }
      struct symbol* fname = symbol_table_scope_lookup(st, e->left->name);
       if (!fcall_compare(st, fname->type->params, e->right)) {
       error_status = expr_type_error_handle(PARAM, (void*)fname, (void*)e->right, NULL, NULL);
       if (!result) result = type_copy(left_expr_type);
      }
      else result = type_copy(left_expr_type->subtype);
      break;

    // intitialization list
    case EXPR_INIT:
      result = expr_typecheck_init(st, e->left);
      if (e->kind == EXPR_INIT) { result = type_create(TYPE_ARRAY, result, NULL, NULL); }
      break;

    // comma
    case EXPR_COMMA:
      result = type_copy(left_expr_type);
      break;

    // assignment
    case EXPR_ASSIGN:
      // operands must be of same type
      if (!type_equals(left_expr_type, right_expr_type)) {
       error_status = expr_type_error_handle(ASSIGN, (void*)e->left, (void*)left_expr_type, (void*)e->right, (void*)right_expr_type);
      }
      // LHS must be LValue
      if (e->left->kind != EXPR_NAME && e->left->kind != EXPR_SUBSCRIPT) {
       error_status = expr_type_error_handle(LVAL, (void*)e->left, (void*)left_expr_type, NULL, NULL);
      }
      // LHS must be mutable
      if (left_expr_type->kind == TYPE_STRING) {
        error_status = expr_type_error_handle(IMMUTABLE, (void*)e->left, (void*)left_expr_type, NULL, NULL);
      }
      else result = type_copy(left_expr_type);
      break;
  }
  type_destroy(&left_expr_type);
  type_destroy(&right_expr_type);
  return result;
}


/*
Generates the corresponding (unoptimized) assembly code for the given expression e.
If any error occurs that is NOT due to register allocation such as but not limited to:
        - division by 0 --> error (fatal)
        - modulus by 0 --> error (fatal)
        - integer underflow --> warning (nonfatal)
        - integer overflow --> warning (nonfatal)
An error/warning code is emitted and send to the error message handler.
*/
int expr_codegen(struct symbol_table* st, struct expr* e) {
  if (!e) { return error_status; } // basis reached.

  // post order traversal, left child, right child, then parent
  error_status = expr_codegen(st, e->left);
  error_status = expr_codegen(st, e->right);

  // parent
  switch(e->kind) {
    // leaf nodes/primitive types
    // load the literal
    case EXPR_INT:
    case EXPR_CH:
    case EXPR_BOOL: /* MOVQ $N, %R*/
      e->reg = register_scratch_alloc();
      if (generate_expr) {
        fprintf(CODEGEN_OUT, "MOVQ $%ld, %s\n", e->literal_value, register_scratch_name(e->reg));
      }
      break;
    // load the address (the associated label name with the literal)
    case EXPR_STR:
      e->reg = register_scratch_alloc();
      if (generate_expr) {
        fprintf(CODEGEN_OUT, "LEAQ %s, %s\n",
                            (const char*)symbol_table_hidden_lookup(st->hidden_table, e->string_literal),
                            register_scratch_name(e->reg));
      }
      break;
    // load the symbol
    case EXPR_NAME:
    /*
    pass-by-value
    MOVQ <name>, %R / MOVQ %rsp(N), %R
    
    pass-by-reference (string, array)
    LEAQ <name>, %R / LEAQ %rps(N), %R
    */
      e->reg = register_scratch_alloc();
      if (generate_expr) {
        fprintf(CODEGEN_OUT, "%s %s, %s\n",
                           (e->symbol->type->kind == TYPE_STRING || e->symbol->type->kind == TYPE_ARRAY)
                           ? "LEAQ" : "MOVQ",
                            symbol_codegen(e->symbol), register_scratch_name(e->reg));
      }
      break;

    // arithmetic + logical operations
    case EXPR_ASSIGN: /* MOVQ %RR, %RL*/
      if (generate_expr) {
        fprintf(CODEGEN_OUT, "MOVQ %s, %s\n", register_scratch_name(e->left->reg), register_scratch_name(e->right->reg));
      }
      register_scratch_free(e->left->reg);
      e->reg = e->right->reg;

      if (!e->string_literal) { e->literal_value = e->right->literal_value; }
      //else { e->string_literal = strdup(e->right->string_literal); }
      break;
    case EXPR_ADD: /* ADDQ %RL, %RR */
      if (generate_expr) {
        fprintf(CODEGEN_OUT, "ADDQ %s, %s\n", register_scratch_name(e->left->reg), register_scratch_name(e->right->reg));
      }
      register_scratch_free(e->left->reg);
      e->reg = e->right->reg;

      // value tracking
      e->literal_value = e->left->literal_value + e->right->literal_value;
      if (flow_check(e->kind, e->literal_value, e->left->literal_value, e->right->literal_value, OVERFLOW)) {
        error_status = expr_codegen_error_handle(ERR_OVERFLOW, e);
      } else if (flow_check(e->kind, e->literal_value, e->left->literal_value, e->right->literal_value, UNDERFLOW)) {
        error_status = expr_codegen_error_handle(ERR_UNDERFLOW, e);
      }
      break;
    case EXPR_SUB: /* SUBQ %RL, %RR */
      if (generate_expr) {
        fprintf(CODEGEN_OUT, "SUBQ %s, %s\n", register_scratch_name(e->left->reg), register_scratch_name(e->right->reg));
      }
      register_scratch_free(e->left->reg);
      e->reg = e->right->reg;

      // value tracking
      e->literal_value = e->left->literal_value - e->right->literal_value;
      if (flow_check(e->kind, e->literal_value, e->left->literal_value, e->right->literal_value, OVERFLOW)) {
        error_status = expr_codegen_error_handle(ERR_OVERFLOW, e);
      } else if (flow_check(e->kind, e->literal_value, e->left->literal_value, e->right->literal_value, UNDERFLOW)) {
        error_status = expr_codegen_error_handle(ERR_UNDERFLOW, e);
      }
      break;
    case EXPR_AND: /* ANDQ %RL, %RR */
      if (generate_expr) {
        fprintf(CODEGEN_OUT, "ANDQ %s, %s\n", register_scratch_name(e->left->reg), register_scratch_name(e->right->reg));
      }
      register_scratch_free(e->left->reg);
      e->reg = e->right->reg;
      e->literal_value = e->left->literal_value && e->right->literal_value;
      break;
    case EXPR_OR: /* ORQ %RL, %RR */
      if (generate_expr) {
        fprintf(CODEGEN_OUT, "ORQ %s, %s\n", register_scratch_name(e->left->reg), register_scratch_name(e->right->reg));
      }
      register_scratch_free(e->left->reg);
      e->reg = e->right->reg;
      e->literal_value = e->left->literal_value || e->right->literal_value;
      break;

    /* "unary" */
    case EXPR_POS: /* nothing happens */
      e->reg = e->left->reg;
      e->literal_value = e->left->literal_value;
      break;
    case EXPR_NEG: /* NEG %RL*/
      if (generate_expr) {
        fprintf(CODEGEN_OUT, "NEGQ %s\n", register_scratch_name(e->left->reg));
      }
      e->reg = e->left->reg;
      e->literal_value = -1 * e->left->literal_value;
      break;
    case EXPR_INC: /* INC %RL */
      if (generate_expr) {
        fprintf(CODEGEN_OUT, "INCQ %s\n", register_scratch_name(e->left->reg));
      }
      e->reg = e->left->reg;

      // value tracking
      e->literal_value = e->left->literal_value + 1;
      if (flow_check(e->kind, e->literal_value, e->left->literal_value, -1, OVERFLOW)) {
        error_status = expr_codegen_error_handle(ERR_OVERFLOW, e);
      } else if (flow_check(e->kind, e->literal_value, e->left->literal_value, -1, UNDERFLOW)) {
        error_status = expr_codegen_error_handle(ERR_UNDERFLOW, e);
      }
      break;
    case EXPR_DEC: /* DEC %RL */
      if (generate_expr) { 
        fprintf(CODEGEN_OUT, "DECQ %s\n", register_scratch_name(e->left->reg));
      }
      e->reg = e->left->reg;

      // value tracking
      e->literal_value = e->left->literal_value - 1;
      if (flow_check(e->kind, e->literal_value, e->left->literal_value, -1, OVERFLOW)) {
        error_status = expr_codegen_error_handle(ERR_OVERFLOW, e);
      } else if (flow_check(e->kind, e->literal_value, e->left->literal_value, -1, UNDERFLOW)) {
        error_status = expr_codegen_error_handle(ERR_UNDERFLOW, e);
      }
      break;
    case EXPR_NOT: /* NOT %RL */
      if (generate_expr) {
        fprintf(CODEGEN_OUT, "NOTQ %s\n", register_scratch_name(e->left->reg));
      }
      e->reg = e->left->reg;
      e->literal_value = !e->left->literal_value;
      break;

    /*
    MOVQ %RL, %RAX
    IMULQ %RR, %RAX
    MOVQ %RAX, %RL
    */
    case EXPR_MULT:
      // allocate different register for resultant since the dest register is not always overwritten.
      e->reg = register_scratch_alloc();
      if (generate_expr) {
        fprintf(CODEGEN_OUT, "MOVQ %s, %rax\n", register_scratch_name(e->left->reg));
        fprintf(CODEGEN_OUT, "IMULQ %s, %rax\n", register_scratch_name(e->right->reg));
        fprintf(CODEGEN_OUT, "MOVQ %rax, %s\n", register_scratch_name(e->reg));
      }
      register_scratch_free(e->left->reg);
      register_scratch_free(e->right->reg);

      // value tracking
      e->literal_value = e->left->literal_value * e->right->literal_value;
      if (flow_check(e->kind, e->literal_value, e->left->literal_value, e->right->literal_value, OVERFLOW)) {
        error_status = expr_codegen_error_handle(ERR_OVERFLOW, e);
      } else if (flow_check(e->kind, e->literal_value, e->left->literal_value, e->right->literal_value, UNDERFLOW)) {
        error_status = expr_codegen_error_handle(ERR_UNDERFLOW, e);
      }
      break;

    /*
    MOVQ %RL, %RAX
    CQTO ; sign extend %RAX TO %RDX:%RAX
    IDIVQ %RR, %RAX
    MOVQ %RAX, %RL ; quotient
    MOVQ %RDX, %RL ; remainder
    */
    case EXPR_DIV:
    case EXPR_MOD:
      e->reg = register_scratch_alloc();
      if (generate_expr) {
        fprintf(CODEGEN_OUT, "MOVQ %s, %rax\nCQTO\n", register_scratch_name(e->left->reg));
        fprintf(CODEGEN_OUT, "IDIVQ %s\n", register_scratch_name(e->right->reg));
        fprintf(CODEGEN_OUT, "MOVQ %s, %s\n", (e->kind == EXPR_DIV) ? "%rax" : "%rdx",
                                            register_scratch_name(e->reg));
      }
      register_scratch_free(e->left->reg);
      register_scratch_free(e->right->reg);

      // value tracking
      if (e->right->literal_value == 0) { return error_status = expr_codegen_error_handle(EXPR_BYZERO, e); }
      if (flow_check(e->kind, e->literal_value, e->left->literal_value, e->right->literal_value, OVERFLOW)) {
        error_status = expr_codegen_error_handle(ERR_OVERFLOW, e);
      } else {
        e->literal_value = (e->kind == EXPR_DIV) ?
                            e->left->literal_value / e->right->literal_value :
                            e->left->literal_value % e->right->literal_value;
      }
      break;
    case EXPR_SUBSCRIPT:

   // relational expressions
   /* CMP %RL, %RR
      SET[] %RL

    // these affect the LOW BYTE registers, therefore extend them:
    MOVZBQ %[low byte register], %RL
   */

  // TODO: have a flag to indicate if part of a condition or not. the root SET[] will be set to a JMP/J[]

   case EXPR_EQ: /* SETE %AL*/
     if (generate_expr) {

      // TODO: this only works for integer cases. use runtime library for strcmp.
      fprintf(CODEGEN_OUT,"CMP %s, %s\nSETE %s\nMOVZBQ %s, %s\n",
             register_scratch_name(e->left->reg), register_scratch_name(e->right->reg),
             register_scratch_name_low(e->left->reg),
             register_scratch_name_low(e->left->reg), register_scratch_name(e->left->reg));
     }
     register_scratch_free(e->right->reg);
     e->reg = e->left->reg;
     if (!e->left->string_literal) { e->literal_value = e->left->literal_value == e->right->literal_value; }
     else{ /* e->literal_value = (strcmp(e->left->string_literal, e->right->string_literal) == 0); */ }
     break;
   case EXPR_NEQ: /* SETNE %AL */
    if (generate_expr) {
      // TODO: this only works for integer cases. use runtime library for strcmp.
     fprintf(CODEGEN_OUT,"CMP %s, %s\nSETNE %s\nMOVZBQ %s, %s\n",
             register_scratch_name(e->left->reg), register_scratch_name(e->right->reg),
             register_scratch_name_low(e->left->reg),
             register_scratch_name_low(e->left->reg), register_scratch_name(e->left->reg));
    }
     register_scratch_free(e->right->reg);
     e->reg = e->left->reg;
     if (!e->left->string_literal) { e->literal_value = (e->left->literal_value != e->right->literal_value); }
     else { /* e->literal_value = (strcmp(e->left->string_literal, e->right->string_literal) != 0);*/}
     break;
   case EXPR_LESS: /* SETL %AL */
     if (generate_expr) {
      fprintf(CODEGEN_OUT,"CMP %s, %s\nSETL %s\nMOVZBQ %s, %s\n",
             register_scratch_name(e->left->reg), register_scratch_name(e->right->reg),
             register_scratch_name_low(e->left->reg),
             register_scratch_name_low(e->left->reg), register_scratch_name(e->left->reg));
     }
     register_scratch_free(e->right->reg);
     e->reg = e->left->reg;
     e->literal_value = (e->left->literal_value < e->right->literal_value);
     break;
   case EXPR_LEQ: /* SETLE %AL */
     if (generate_expr) {
      fprintf(CODEGEN_OUT,"CMP %s, %s\nSETLE %s\nMOVZBQ %s, %s\n",
             register_scratch_name(e->left->reg), register_scratch_name(e->right->reg),
             register_scratch_name_low(e->left->reg),
             register_scratch_name_low(e->left->reg), register_scratch_name(e->left->reg));
     }
     register_scratch_free(e->right->reg);
     e->reg = e->left->reg;
     e->literal_value = (e->left->literal_value <= e->right->literal_value);
     break;
   case EXPR_GREAT: /* SETG %AL */
     if (generate_expr) {
      fprintf(CODEGEN_OUT,"CMP %s, %s\nSETG %s\nMOVZBQ %s, %s\n",
             register_scratch_name(e->left->reg), register_scratch_name(e->right->reg),
             register_scratch_name_low(e->left->reg),
             register_scratch_name_low(e->left->reg), register_scratch_name(e->left->reg));
     }
     register_scratch_free(e->right->reg);
     e->reg = e->left->reg;
     e->literal_value = (e->left->literal_value > e->right->literal_value);
     break;
   case EXPR_GEQ: /* SETGE %AL */
     if (generate_expr) {
      fprintf(CODEGEN_OUT,"CMP %s, %s\nSETGE %s\nMOVZBQ %s, %s\n",
             register_scratch_name(e->left->reg), register_scratch_name(e->right->reg),
             register_scratch_name_low(e->left->reg),
             register_scratch_name_low(e->left->reg), register_scratch_name(e->left->reg));
     }
     register_scratch_free(e->right->reg);
     e->reg = e->left->reg;
     e->literal_value =(e->left->literal_value >= e->right->literal_value);
     break;


   // may need their own helper functions
   case EXPR_EXP:
      // will be like fcall in (de)allocating registers before and after call.
      // will call runtime library function exp() instead of having a statement body to just generate.
   case EXPR_INIT: // this expression only occurs in declarations
   case EXPR_COMMA: // TODO: test multiple register allocations here
   case EXPR_FCALL:
    /*
    TO DO:
    1. generate allocation of registers prior to call (test with stub function body)
    2. generate deallocation of registers after call (test with stub function body)
    3. generate function prologue
    4. generate function epilogue
    5. generate function body
    */
   default: break; // shouldnt come here but throw error?????
  }
}
