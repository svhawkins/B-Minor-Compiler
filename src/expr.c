#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "expr.h"

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

  // // free the register
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
  if (!st || !e) return 0;
  int error_status = 0;
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
    label_name(label_create()); // stored in global label_str
    e->symbol = symbol_create(SYMBOL_HIDDEN, type_create(TYPE_STRING, NULL, NULL, NULL), label_str);
    symbol_table_scope_bind(st, label_str, e->symbol);

    // // add it to hidden delcarations
    // struct decl* d = decl_create(strdup(label_str), type_create(TYPE_STRING, NULL, NULL, NULL), expr_copy(e), NULL, NULL);
    // if (!decl_hidden_list) { decl_hidden_list = d; }
    // else { decl_hidden_list_tail->next = d; }
    // decl_hidden_list_tail = d;

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
        - division by 0
        - modulus by 0
        - integer underflow
        - integer overflow
An error code is emitted and send to the error message handler.
*/
int expr_codegen(struct expr* e) {
  if (!e) { return 0; } // basis reached.

  // post order traversal, left child, right child, then parent
  error_status = expr_codegen(e->left);
  error_status = expr_codegen(e->right);

  // parent
  switch(e->kind) {
    // leaf nodes/primitive types
    // load the literal
    case EXPR_INT:
    case EXPR_CH:
    case EXPR_BOOL: /* MOVQ $N, %R*/
      e->reg = register_scratch_alloc();
      fprintf(CODEGEN_OUT, "MOVQ $%ld, %s\n", e->literal_value, register_scratch_name(e->reg));
      break;
    // load the address
    case EXPR_STR:
      // TO DO: this solution only works for strings stored as variables. what about string literals?
      e->reg = register_scratch_alloc();
      fprintf(CODEGEN_OUT, "LEAQ %s, %s\n", symbol_codegen(e->symbol), register_scratch_name(e->reg));
      break;
    // load the symbol
    case EXPR_NAME: /* MOVQ <name>, %R / MOVQ %rsp(N), %R */
      // TO DO: what if the symbol is an array or string? then LEAQ!
      e->reg = register_scratch_alloc();
      fprintf(CODEGEN_OUT, "MOVQ %s, %s\n", symbol_codegen(e->symbol), register_scratch_name(e->reg));
      break;

    // arithmetic + logical operations
    case EXPR_ASSIGN: /* MOVQ %RR, %RL*/
      fprintf(CODEGEN_OUT, "MOVQ %s, %s\n", register_scratch_name(e->left->reg), register_scratch_name(e->right->reg));
      register_scratch_free(e->left->reg);
      e->reg = e->right->reg;
      break;
    case EXPR_ADD: /* ADDQ %RL, %RR */
      fprintf(CODEGEN_OUT, "ADDQ %s, %s\n", register_scratch_name(e->left->reg), register_scratch_name(e->right->reg));
      register_scratch_free(e->left->reg);
      e->reg = e->right->reg;
      break;
    case EXPR_SUB: /* SUBQ %RL, %RR */
      fprintf(CODEGEN_OUT, "SUBQ %s, %s\n", register_scratch_name(e->left->reg), register_scratch_name(e->right->reg));
      register_scratch_free(e->left->reg);
      e->reg = e->right->reg;
      break;
    case EXPR_AND: /* ANDQ %RL, %RR */
      fprintf(CODEGEN_OUT, "ANDQ %s, %s\n", register_scratch_name(e->left->reg), register_scratch_name(e->right->reg));
      register_scratch_free(e->left->reg);
      e->reg = e->right->reg;
      break;
    case EXPR_OR: /* ORQ %RL, %RR */
      fprintf(CODEGEN_OUT, "ORQ %s, %s\n", register_scratch_name(e->left->reg), register_scratch_name(e->right->reg));
      register_scratch_free(e->left->reg);
      e->reg = e->right->reg;
      break;

    /* "unary" */
    case EXPR_POS: /* nothing happens */
      e->reg = e->left->reg;
      break;
    case EXPR_NEG: /* NEG %RL*/
      fprintf(CODEGEN_OUT, "NEGQ %s\n", register_scratch_name(e->left->reg));
      e->reg = e->left->reg;
      break;
    case EXPR_INC: /* INC %RL */
      fprintf(CODEGEN_OUT, "INCQ %s\n", register_scratch_name(e->left->reg));
      e->reg = e->left->reg;
      break;
    case EXPR_DEC: /* DEC %RL */
      fprintf(CODEGEN_OUT, "DECQ %s\n", register_scratch_name(e->left->reg));
      e->reg = e->left->reg;
      break;
    case EXPR_NOT: /* NOT %RL */
      fprintf(CODEGEN_OUT, "NOTQ %s\n", register_scratch_name(e->left->reg));
      e->reg = e->left->reg;
      break;

    // bit harder
    case EXPR_MULT:
    case EXPR_DIV:
    case EXPR_MOD:
    case EXPR_SUBSCRIPT:

   // relational expressions
   /* CMP %RL, %RR
      SET[] %RL

    // these affect the LOW BYTE registers, therefore extend them:
    MOVZBQ %[low byte register], %RL
   */

  // TODO: have a flag to indicate if part of a condition or not. the root SET[] will be set to a JMP/J[]

   case EXPR_EQ: /* SETE %AL*/
     fprintf(CODEGEN_OUT,"CMP %s, %s\nSETE %s\nMOVZBQ %s, %s\n",
             register_scratch_name(e->left->reg), register_scratch_name(e->right->reg),
             register_scratch_name_low(e->left->reg),
             register_scratch_name_low(e->left->reg), register_scratch_name(e->left->reg));
     register_scratch_free(e->right->reg);
     e->reg = e->left->reg;
     break;
   case EXPR_NEQ: /* SETNE %AL */
     fprintf(CODEGEN_OUT,"CMP %s, %s\nSETNE %s\nMOVZBQ %s, %s\n",
             register_scratch_name(e->left->reg), register_scratch_name(e->right->reg),
             register_scratch_name_low(e->left->reg),
             register_scratch_name_low(e->left->reg), register_scratch_name(e->left->reg));
     register_scratch_free(e->right->reg);
     e->reg = e->left->reg;
     break;
   case EXPR_LESS: /* SETL %AL */
     fprintf(CODEGEN_OUT,"CMP %s, %s\nSETL %s\nMOVZBQ %s, %s\n",
             register_scratch_name(e->left->reg), register_scratch_name(e->right->reg),
             register_scratch_name_low(e->left->reg),
             register_scratch_name_low(e->left->reg), register_scratch_name(e->left->reg));
     register_scratch_free(e->right->reg);
     e->reg = e->left->reg;
     break;
   case EXPR_LEQ: /* SETLE %AL */
     fprintf(CODEGEN_OUT,"CMP %s, %s\nSETLE %s\nMOVZBQ %s, %s\n",
             register_scratch_name(e->left->reg), register_scratch_name(e->right->reg),
             register_scratch_name_low(e->left->reg),
             register_scratch_name_low(e->left->reg), register_scratch_name(e->left->reg));
     register_scratch_free(e->right->reg);
     e->reg = e->left->reg;
     break;
   case EXPR_GREAT: /* SETG %AL */
     fprintf(CODEGEN_OUT,"CMP %s, %s\nSETG %s\nMOVZBQ %s, %s\n",
             register_scratch_name(e->left->reg), register_scratch_name(e->right->reg),
             register_scratch_name_low(e->left->reg),
             register_scratch_name_low(e->left->reg), register_scratch_name(e->left->reg));
     register_scratch_free(e->right->reg);
     e->reg = e->left->reg;
     break;
   case EXPR_GEQ: /* SETGE %AL */
     fprintf(CODEGEN_OUT,"CMP %s, %s\nSETGE %s\nMOVZBQ %s, %s\n",
             register_scratch_name(e->left->reg), register_scratch_name(e->right->reg),
             register_scratch_name_low(e->left->reg),
             register_scratch_name_low(e->left->reg), register_scratch_name(e->left->reg));
     register_scratch_free(e->right->reg);
     e->reg = e->left->reg;
     break;


   // may need their own helper functions
   case EXPR_EXP:
   case EXPR_INIT:
   case EXPR_COMMA: // TODO: test multiple register allocations here
   default: break; // shouldnt come here but throw error?????
  }
}
