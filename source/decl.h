#ifndef DECL_H
#define DECL_H

#include "type.h"
#include "stmt.h"
#include "expr.h"
#include "symbol.h"
#include "symbol_table.h"
#include "register.h"
struct symbol_table; // forward decl for reasons
typedef enum { DECL_NULL = 1, DECL_NINT } decl_error_t;
int decl_error_handle(decl_error_t kind, void* ctx1, void* ctx2);


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
*/
int decl_resolve(struct symbol_table* st, struct decl* d);

int decl_typecheck(struct symbol_table*, struct decl* d);

#endif

