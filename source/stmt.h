#ifndef STMT_H
#define STMT_H

#include "decl.h"

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

#endif
