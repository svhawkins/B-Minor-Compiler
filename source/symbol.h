#ifndef SYMBOL_H
#define SYMBOL_H

#include "type.h"
#include "register.h"

typedef enum {
	SYMBOL_LOCAL,
	SYMBOL_PARAM,
	SYMBOL_GLOBAL
} symbol_t;

struct symbol {
	symbol_t kind;
	struct type* type;
	char* name;
	int which;
	bool defined; // for function definitons
};

struct symbol* symbol_create(symbol_t kind, struct type *type, char *name);
void symbol_destroy(struct symbol** s);
void symbol_fprint(FILE* fp, struct symbol* s);

struct symbol* symbol_copy(struct symbol* s);

#endif
