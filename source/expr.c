#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "expr.h"

// helper functions
bool expr_is_primitive(expr_t kind) { return (kind >= EXPR_NAME && kind <= EXPR_INT); }
bool expr_is_unary(expr_t kind) { return (kind >= EXPR_INC && kind <= EXPR_NOT); }
bool expr_is_binary(expr_t kind) { return (kind >= EXPR_EXP && kind <= EXPR_COMMA); }
// excludes subscript [] and fcall () since those have right subtree within operator.

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

    // operators: unary, binary, also different types of associativity make for different placement of parentheses
    // TO DO: associativity testing

    // unary operators
    case EXPR_INC: expr_fprint(fp, e->left); fprintf(fp, "++"); break;
    case EXPR_DEC: expr_fprint(fp, e->left); fprintf(fp, "--"); break;
    case EXPR_POS: fprintf(fp, "+"); expr_fprint(fp, e->left); break; // +45 is just 45.
    case EXPR_NEG: fprintf(fp, "-"); expr_fprint(fp, e->left); break;
    case EXPR_NOT: fprintf(fp, "!"); expr_fprint(fp, e->left); break;

    // binary operators
    case EXPR_EXP: expr_fprint(fp, e->left); fprintf(fp, "^"); expr_fprint(fp, e->right); break;
    case EXPR_MULT: expr_fprint(fp, e->left); fprintf(fp, " * "); expr_fprint(fp, e->right); break;
    case EXPR_DIV: expr_fprint(fp, e->left); fprintf(fp, " / "); expr_fprint(fp, e->right); break;
    case EXPR_MOD: expr_fprint(fp, e->left); fprintf(fp, " %% "); expr_fprint(fp, e->right); break;
    case EXPR_ADD: expr_fprint(fp, e->left); fprintf(fp, " + "); expr_fprint(fp, e->right); break;
    case EXPR_SUB: expr_fprint(fp, e->left); fprintf(fp, " - "); expr_fprint(fp, e->right); break;
    case EXPR_LEQ: expr_fprint(fp, e->left); fprintf(fp, " <= "); expr_fprint(fp, e->right); break;
    case EXPR_LESS: expr_fprint(fp, e->left); fprintf(fp, " < "); expr_fprint(fp, e->right); break;
    case EXPR_GEQ: expr_fprint(fp, e->left); fprintf(fp, " >= "); expr_fprint(fp, e->right); break;
    case EXPR_GREAT: expr_fprint(fp, e->left); fprintf(fp, " > "); expr_fprint(fp, e->right); break;
    case EXPR_EQ: expr_fprint(fp, e->left); fprintf(fp, " == "); expr_fprint(fp, e->right); break;
    case EXPR_NEQ: expr_fprint(fp, e->left); fprintf(fp, " != "); expr_fprint(fp, e->right); break;
    case EXPR_AND: expr_fprint(fp, e->left); fprintf(fp, " && "); expr_fprint(fp, e->right); break;
    case EXPR_OR: expr_fprint(fp, e->left); fprintf(fp, " || "); expr_fprint(fp, e->right); break;
    case EXPR_ASSIGN: expr_fprint(fp, e->left); fprintf(fp, " = "); expr_fprint(fp, e->right); break;
    case EXPR_COMMA: expr_fprint(fp, e->left); fprintf(fp, ", "); expr_fprint(fp, e->right); break;

    // also binary operators
    case EXPR_SUBSCRIPT: expr_fprint(fp, e->left); fprintf(fp, "["); expr_fprint(fp, e->right); fprintf(fp, "]"); break;
    case EXPR_FCALL: expr_fprint(fp, e->left); fprintf(fp, "("); expr_fprint(fp, e->right); fprintf(fp, ")"); break;

  }
}

void expr_print(struct expr* e) { expr_fprint(stdout, e); }
