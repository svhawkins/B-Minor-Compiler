#ifndef TYPE_H
#define TYPE_H

#include "param_list.h"
#include "expr.h"

typedef enum {
	TYPE_VOID,
	TYPE_BOOLEAN,
	TYPE_CHARACTER,
	TYPE_INTEGER,
	TYPE_STRING,
	TYPE_AUTO,
	TYPE_ARRAY,
	TYPE_FUNCTION,
} type_t;

struct type {
	type_t kind;
	struct param_list* params;
	struct type* subtype;
	struct expr* size; // used to properly print arrays
};

struct type* type_create(type_t kind, struct type* subtype, struct param_list* params, struct expr* size);

void type_fprint(FILE* fp, struct type* t);
void type_print(struct type* t);


// assigns the leaf subtype a subtype
void type_subtype_leaf_assign(struct type* t, struct type* subtype);

void type_destroy(struct type** t);

#endif
