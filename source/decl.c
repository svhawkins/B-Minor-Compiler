#include <stdio.h>
#include <stdlib.h>
#include "decl.h"

const char* SPACE = "  ";
struct decl* decl_create(char* name, struct type* type, struct expr* value, struct stmt* code, struct decl* next)
{
  struct decl* d = malloc(sizeof(*d));
  if (d) {
    d->name = name;
    d->type = type;
    d->value = value;
    d->code = code;
    d->next = next;
  }
  return d;
}


// have helper function to print out array initializations
void init_fprint(FILE* fp, struct decl* d, struct type* t) {}
void decl_fprint(FILE* fp, struct decl* d, int indent) {
  if (!d) return;

  // print indentation
  for (int i = 0; i < indent; i++) fprintf(fp, "%s", SPACE);

  // name : type
  fprintf(fp, "%s: ", d->name); type_fprint(fp, d->type);

  // init or uninit?
  switch(d->type->kind) {
  case TYPE_FUNCTION:
    if (d->code) { fprintf(fp, " = "); stmt_fprint(fp, d->code, indent++); break; }
    else fprintf(fp, ";\n");
    break;
  default:
    if (d->value) {
      fprintf(fp, " = ");
      if (d->type->kind != TYPE_ARRAY) expr_fprint(fp, d->value);
    }
    fprintf(fp, ";\n");
    break;
  }
  decl_fprint(fp, d->next, indent);
}
void decl_print(struct decl* d, int indent) { decl_fprint(stdout, d, indent); }
