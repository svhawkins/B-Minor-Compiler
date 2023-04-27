#ifndef DECL_H
#define DECL_H

#include "type.h"
#include "stmt.h"
#include "expr.h"
#include "symbol.h"
#include "symbol_table.h"
struct symbol_table; // forward decl for reasons


struct decl {
	char* name;
	struct type* type;
	struct expr* value;
	struct stmt* code;
	struct symbol* symbol;
	struct decl* next;
};

struct decl* decl_create(char* name, struct type* type, struct expr* value, struct stmt* code, struct decl* next);

void decl_fprint(FILE* fp, struct decl* d, int indent);
void decl_print(struct decl* d, int indent);

void decl_destroy(struct decl** d);

/*
Adds symbols to the symbol table
Error messages:
*/
void decl_resolve(struct symbol_table* st, struct decl* d);

#endif

