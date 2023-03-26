#include <stdio.h>
#include <stdlib.h>
#include "param_list.h"

struct param_list* param_list_create(char* name, struct type* type, struct param_list* next )
{
  struct param_list* p = malloc(sizeof(*p));
  if (p) {
    p->name = name;
    p->type = type;
    p->next = next;
  }
  return p;
}
void param_list_print(struct param_list *p) {}
