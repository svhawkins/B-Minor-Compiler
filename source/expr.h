#ifndef EXPR_H
#define EXPR_H

#include "symbol.h"
#include "register.h"
#include "symbol_table.h"
struct symbol_table; // forward decl for reasons
typedef enum { MATH = 1, RELATE, LOGIC, EQUAL, ASSIGN, LVAL, INIT, SUBSCRIPT, FCALL, PARAM } type_error_t;
typedef enum {
        // unary operators
	EXPR_INC = 0, // l++
	EXPR_DEC,// l--
	EXPR_POS, // +l
	EXPR_NEG, // -l
	EXPR_NOT, // !l

	// binary operators
        EXPR_EXP, // l^r
        EXPR_MULT, // l*r
        EXPR_DIV, // l/r
        EXPR_MOD, // l%r
        EXPR_ADD, // l+r
	EXPR_SUB, // l-r
        EXPR_LEQ, // l<=r
        EXPR_LESS, // l<r
        EXPR_GEQ, // l>=r
        EXPR_GREAT, // l>r
        EXPR_EQ, // l==r
        EXPR_NEQ, // l!=r
        EXPR_AND, // l &&r
        EXPR_OR, // l || r
        EXPR_ASSIGN, // l = r
	EXPR_COMMA, // l, r

        // operators with stuff in the middle
        EXPR_SUBSCRIPT, // l[r]
        EXPR_FCALL, // l(r)
	EXPR_INIT, // {l}

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

	/* register allocation */
	int reg; // index for scratch registers
};

struct expr* expr_create(expr_t kind, struct expr* left, struct expr* right);

struct expr* expr_create_name(const char* n);
struct expr* expr_create_integer_literal(int c);
struct expr* expr_create_boolean_literal(int c);
struct expr* expr_create_char_literal(char c);
struct expr* expr_create_string_literal(const char* str);

void expr_fprint(FILE* fp, struct expr* e);
void expr_print(struct expr* e);

void expr_destroy(struct expr** e);

struct expr* expr_copy(struct expr* e);

/*
Looks up symbols in the symbol table.
*/
int expr_resolve(struct symbol_table* st, struct expr* e);

/*
Typechecks an expression
Returns the type structure the expression ultimately is.
In cases of error:
	- failure/null expressions return NULL as a type
	- errors return the type of the expression in the left branch of the expression tree
*/
struct type* expr_typecheck(struct symbol_table* st, struct expr* e);


/*
Generates the corresponding (unoptimized) assembly code for the given expression e.
If any error occurs that is NOT due to register allocation such as but not limited to:
	- division by 0
	- modulus by 0
	- integer underflow
	- integer overflow
An error code is emitted and send to the error message handler.
*/
int expr_codegen(struct expr* e);

#endif
