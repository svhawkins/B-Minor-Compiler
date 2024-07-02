#ifndef SYMBOL_H
#define SYMBOL_H

#include "type.h"
#include "register.h"
#define MAX_LENGTH 256 // maximum length of variable name and more than enough digits!
char symbol_address[MAX_LENGTH];

typedef enum {
	SYMBOL_GLOBAL,
	SYMBOL_PARAM,
	SYMBOL_LOCAL,
} symbol_t;

struct symbol {
	symbol_t kind;
	struct type* type;
	char* name;
	int which;
	bool defined; // for function definitions (and i guess declarations without values)
	char* address; // for symbol code generation
};

struct symbol* symbol_create(symbol_t kind, struct type *type, char *name);
void symbol_destroy(struct symbol** s);
void symbol_fprint(FILE* fp, struct symbol* s);

struct symbol* symbol_copy(struct symbol* s);

/* generates the proper address/label for a symbol */
const char* symbol_codegen(struct symbol* s);
const char* symbol_codegen_offset(struct symbol* s, int offset);


#endif /* SYMBOL_H */
