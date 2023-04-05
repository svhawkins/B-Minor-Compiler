#ifndef EXPR_H
#define EXPR_H

#include "symbol.h"

typedef enum {
        // binary operators same as the tokens
	EXPR_INC = 0,
	EXPR_DEC,
	EXPR_POS,
	EXPR_NEG,
	EXPR_NOT,
        EXPR_EXP,
        EXPR_MULT,
        EXPR_DIV,
        EXPR_MOD,
        EXPR_ADD,
	EXPR_SUB,
        EXPR_LEQ,
        EXPR_LESS,
        EXPR_GEQ,
        EXPR_GREAT,
        EXPR_EQ,
        EXPR_NEQ,
        EXPR_AND,
        EXPR_OR,
        EXPR_ASSIGN,

        // operators with stuff in the middle
        EXPR_SUBSCRIPT,
        EXPR_FCALL,

        // primitives
        EXPR_NAME,
        EXPR_CH,
        EXPR_BOOL,
        EXPR_STR,
        EXPR_INT
} expr_t;

struct expr {
	/* used by all kinds of exprs */
	expr_t kind;
	struct expr* left;
	struct expr* right;

	/* used by various leaf exprs */
	const char* name;
	int literal_value;
	const char* string_literal;
	struct symbol* symbol;
};

struct expr* expr_create(expr_t kind, struct expr* left, struct expr* right);

struct expr* expr_create_name(const char* n);
struct expr* expr_create_integer_literal(int c);
struct expr* expr_create_boolean_literal(int c);
struct expr* expr_create_char_literal(char c);
struct expr* expr_create_string_literal(const char* str);

void expr_fprint(FILE* fp, struct expr* e);
void expr_print(struct expr* e);

#endif
