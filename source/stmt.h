#ifndef STMT_H
#define STMT_H

#include "decl.h"
#include "symbol_table.h"
struct symbol_table; // forward decl for reasons

typedef enum {
	STMT_DECL,
	STMT_EXPR,
	STMT_IF_ELSE,
	STMT_FOR,
	STMT_PRINT,
	STMT_RETURN,
	STMT_BLOCK,
	STMT_WHILE,
} stmt_t;

struct stmt {
	stmt_t kind;
	struct decl* decl;
	struct expr* init_expr;
	struct expr* expr;
	struct expr* next_expr;
	struct stmt* body;
	struct stmt* else_body;
	struct stmt* next;
};

struct stmt* stmt_create(stmt_t kind, struct decl* decl,
                           struct expr* init_expr, struct expr* expr, struct expr* next_expr,
                           struct stmt* body, struct stmt* else_body,
                           struct stmt* next);

void stmt_fprint(FILE* fp, struct stmt* s, int indent);
void stmt_print(struct stmt* s, int indent);

void stmt_destroy(struct stmt** s);

/*
Looks up and adds symbols to table based on the statement kind given.
*/
void stmt_resolve(struct symbol_table* st, struct stmt* s);

void stmt_typecheck(struct symbol_table* st, struct stmt* s);

#endif
