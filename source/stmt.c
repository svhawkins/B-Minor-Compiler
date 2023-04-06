#include <stdio.h>
#include <stdlib.h>
#include "stmt.h"

extern const char* SPACE;
struct stmt* stmt_create(stmt_t kind, struct decl* decl,
			 struct expr* init_expr, struct expr* expr, struct expr* next_expr,
                         struct stmt* body, struct stmt* else_body, struct stmt* next)
{
  struct stmt* s = malloc(sizeof(*s));
  if (s) {
    s->kind = kind;
    s->decl = decl;
    s->init_expr = init_expr;
    s->expr = expr;
    s->next_expr = next_expr;
    s->body = body;
    s->else_body = else_body;
    s->next = next;
  }
  return s;
}

void print_indent(FILE* fp, int indent) { for (int i = 0; i < indent; i++) fprintf(fp, "%s", SPACE); }
void stmt_fprint(FILE* fp, struct stmt* s, int indent) {
  if (!s) return;
  print_indent(fp, indent);

  // types of statements
  switch(s->kind) {
    case STMT_DECL: decl_fprint(fp, s->decl, indent); break;
    case STMT_EXPR: expr_fprint(fp, s->expr); fprintf(fp, ";\n"); break;
    case STMT_IF_ELSE: break;
    case STMT_FOR: break;
    case STMT_PRINT: break;
    case STMT_RETURN: break;
    case STMT_BLOCK: fprintf(fp, "{\n"); stmt_fprint(fp, s->body, indent++);
		     print_indent(fp, indent); fprintf(fp, "\n}\n"); break;
    case STMT_WHILE: break;
  }
  stmt_fprint(fp, s->next, indent);
}
void stmt_print(struct stmt* s, int indent) { stmt_fprint(stdout, s, indent); }
