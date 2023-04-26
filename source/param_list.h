#ifndef PARAM_LIST_H
#define PARAM_LIST_H

#include <stdbool.h>
#include "type.h"
//struct expr;

struct param_list {
	char* name;
	struct type* type;
	struct symbol* symbol;
	struct param_list* next;
};

struct param_list* param_list_create(char* name, struct type* type, struct param_list* next);

void param_list_fprint(FILE* fp, struct param_list* p);
void param_list_print(struct param_list* p);

void param_list_destroy(struct param_list** p);

struct param_list* param_list_copy(struct param_list* p);

bool param_list_equals(struct param_list* a, struct param_list* b);

#endif
