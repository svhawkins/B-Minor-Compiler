#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "expr.h"

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
    if (is_expr_list && !type_equals(lt, rt)) { /* TO DO: error message, inequal expression types */ }
    type_destroy(&rt);
    return lt;
}

struct expr* expr_create(expr_t kind, struct expr* left, struct expr* right )
{
  struct expr* e = malloc(sizeof(*e));
  if (e) {
    e->kind = kind;
    e->left = left;
    e->right = right;

    // non-leaf expressions recieve default values
    e->name = NULL;
    e->literal_value = 0;
    e->string_literal = NULL;
    e->symbol = NULL;
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

struct expr* expr_create_integer_literal(int c)
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
  // what is printed is kind dependent
  if (!e) return;

  switch(e->kind) {
    // primitives
    case EXPR_NAME: fprintf(fp, "%s", e->name); break;
    case EXPR_CH: fprintf(fp, "'%c'", e->literal_value); break;
    case EXPR_BOOL: fprintf(fp, "%s", (e->literal_value) ? "true" : "false"); break;
    case EXPR_STR: fprintf(fp, "\"%s\"", e->string_literal); break;
    case EXPR_INT: fprintf(fp, "%d", e->literal_value); break;

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
  if (!(*e)) return;
  expr_destroy(&((*e)->left));
  expr_destroy(&((*e)->right));
  if ((*e)->kind == EXPR_NAME) free((void*)(*e)->name);
  //symbol_destroy(&((*e)->symbol));
  free(*e); *e = NULL;
}

struct expr* expr_copy(struct expr* e) {
  if (!e) return NULL;
  struct expr* copy = malloc(sizeof(*copy));
  if (copy) {
    copy->kind = e->kind;
    if (e->kind == EXPR_NAME) {
      copy->name = strdup(e->name);
      copy->symbol = symbol_copy(e->symbol);
    }
    copy->left = expr_copy(e->left);
    copy->right = expr_copy(e->right);
  }
  return copy;
}

int expr_resolve(struct symbol_table* st, struct expr* e) {
  if (!st || !e) return 0;
  int error_status = 0;
  if (e->kind == EXPR_NAME) {
    e->symbol = symbol_table_scope_lookup(st, e->name);
    if (!e->symbol) {
      // not found anywhere. undefined reference to <symbol>
      // TO DO: make error message
    }
  } else {
    error_status = expr_resolve(st, e->left) + expr_resolve(st, e->right);
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
     if (!s) { /* TO DO: error message, recover by adding it to table */ }
     result = type_copy(s->type);
     break;

    // equality operators
    case EXPR_EQ:
    case EXPR_NEQ:
      if (!type_equals(left_expr_type, right_expr_type)) { /* TO DO: error message, type mismatch */ }
      else if (invalid_type(left_expr_type->kind) || invalid_type(right_expr_type->kind)) { /* TO DO: error message, incompatible operand type(s) */ }
      else { result = type_create(TYPE_BOOLEAN, NULL, NULL, NULL); }
      break;

    // relational operators
    case EXPR_GREAT:
    case EXPR_GEQ:
    case EXPR_LESS:
    case EXPR_LEQ:
      if (!type_equals(left_expr_type, right_expr_type)) { /* TO DO: error message, type mismatch*/ }
      if (left_expr_type->kind != TYPE_INTEGER || right_expr_type->kind != TYPE_INTEGER) { /* TO DO: error message, incompatible operand type(s) */ }
      else result = type_create(TYPE_BOOLEAN, NULL, NULL, NULL);
      break;

    // logical operators
    case EXPR_AND:
    case EXPR_OR:
      if (!type_equals(left_expr_type, right_expr_type)) { /* TO DO: error message, type mismatch*/ }
      if (left_expr_type->kind != TYPE_BOOLEAN || right_expr_type->kind != TYPE_BOOLEAN) { /* TO DO: error message, incompatible operand type(s) */ }
      else result = type_create(TYPE_BOOLEAN, NULL, NULL, NULL);
      break;
    case EXPR_NOT:
      if (left_expr_type->kind != TYPE_BOOLEAN ) { /* TO DO: error message, incompatible operand type(s) */ }
      else result = type_create(TYPE_BOOLEAN, NULL, NULL, NULL);
      break;

    // arithmetic operators (unary)
    case EXPR_POS:
    case EXPR_NEG:
    case EXPR_INC:
    case EXPR_DEC:
      if (left_expr_type->kind != TYPE_INTEGER ) { /* TO DO: error message, incompatible operand type(s) */ }
      else result = type_create(TYPE_INTEGER, NULL, NULL, NULL);
      break;

    // arithmetic operators (binary)
    case EXPR_ADD:
    case EXPR_SUB:
    case EXPR_MULT:
    case EXPR_DIV:
    case EXPR_MOD:
    case EXPR_EXP:
      if (!type_equals(left_expr_type, right_expr_type)) { /* TO DO: error message, type mismatch*/ }
      if (left_expr_type->kind != TYPE_INTEGER || right_expr_type->kind != TYPE_INTEGER) { /* TO DO: error message, incompatible operand type(s) */ }
      else result = type_create(TYPE_INTEGER, NULL, NULL, NULL);
      break;

    // subscript
    case EXPR_SUBSCRIPT:
      if (left_expr_type->kind != TYPE_ARRAY) { /* TO DO: error message, incompatible operand type(s) */ }
      if (right_expr_type->kind != TYPE_INTEGER) { /* TO DO: error message, incompatible operand type(s) */ }
      else result = type_copy(left_expr_type->subtype);
      break;

    // function call
    case EXPR_FCALL:
      if (left_expr_type->kind != TYPE_FUNCTION) { /* TO DO: error message, incompatible operand type(s) */ }
      struct symbol* fname = symbol_table_scope_lookup(st, e->left->name);
      if (!fcall_compare(st, fname->type->params, e->right)) { /* TO DO: error message of incompatible arguments for parameters */ }
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
      if (e->left->kind != EXPR_NAME || e->left->kind != EXPR_SUBSCRIPT) { /* TO DO: error message, left operand must be lvalue */ }
      if (!type_equals(left_expr_type, right_expr_type)) { /* TO DO: error message */ }
      else result = type_copy(left_expr_type);
      break;
  }
  type_destroy(&left_expr_type);
  type_destroy(&right_expr_type);
  return result;
}
