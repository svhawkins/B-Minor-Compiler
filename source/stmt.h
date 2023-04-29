#ifndef STMT_H
#define STMT_H

#include "decl.h"
#include "symbol_table.h"
struct type;
struct symbol_table; // forward decl for reasons
typedef enum { STMT_BOOLEAN = 1, STMT_TYPE } stmt_error_t;
int stmt_error_handle(stmt_error_t kind, void* ctx1, void* ctx2);

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
int stmt_resolve(struct symbol_table* st, struct stmt* s);

int stmt_typecheck(struct symbol_table* st, struct stmt* s, struct type** ret_type);

#endif
