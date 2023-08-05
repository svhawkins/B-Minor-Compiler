#ifndef SYMBOL_H
#define SYMBOL_H

#include "type.h"
#include "register.h"

typedef enum {
	SYMBOL_LOCAL,
	SYMBOL_PARAM,
	SYMBOL_GLOBAL,
	SYMBOL_HIDDEN // for arrays and strings during code generation. considered global.
} symbol_t;

struct symbol {
	symbol_t kind;
	struct type* type;
	char* name;
	int which;
	bool defined; // for function definitons
	char* address; // for symbol code generation
};

struct symbol* symbol_create(symbol_t kind, struct type *type, char *name);
void symbol_destroy(struct symbol** s);
void symbol_fprint(FILE* fp, struct symbol* s);

struct symbol* symbol_copy(struct symbol* s);

/* generates the proper address/label for a symbol */
const char* symbol_codegen(struct symbol* s);


#endif
