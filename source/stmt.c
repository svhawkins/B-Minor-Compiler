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
  if (!(s->kind == STMT_BLOCK && !s->next)) print_indent(fp, indent);
  // ^ indents are only printed if there is a statement to be printed!

  // types of statements
  switch(s->kind) {

    // decl remains unchanged, no additional indentation.
    case STMT_DECL: decl_fprint(fp, s->decl, 0); break;

    // expr;
    case STMT_EXPR: expr_fprint(fp, s->expr); fprintf(fp, ";"); break;

   /* if (expr) {
        body
      } else {
	else_body
      }
   */
    case STMT_IF_ELSE: fprintf(fp, "if ("); expr_fprint(fp, s->expr); fprintf(fp, ") ");
                       if (s->body && s->body->kind == STMT_BLOCK) {
		         stmt_fprint(fp, s->body, indent); // blocks already have {}
		       } else {
			 fprintf(fp, "{\n"); stmt_fprint(fp, s->body, indent+1);
			 print_indent(fp, indent); fprintf(fp, "}");
		       }
                       if (s->else_body) {
                         if (s->else_body->kind == STMT_BLOCK) {
			   fprintf(fp, " else "); stmt_fprint(fp, s->else_body, indent);
			 } // { body }
                         else {
			   fprintf(fp, " else {\n"); stmt_fprint(fp, s->else_body, indent+1);
			   print_indent(fp, indent); fprintf(fp, "}");
			 }
		       }
		       break;

    /*
      for (init_expr; expr; next_expr) {
        body
      }
      **if init_expr is empty and decl is empty, print a space instead.
      **if init_expr is empty and decl is not empty, print a decl instead (no indentation).
	   decls dont need the semicolon.
   */
    case STMT_FOR: fprintf(fp, "for (");
		   if (!s->init_expr && !s->decl) { fprintf(fp, " ;"); }
		   else if (!s->init_expr && s->decl) { decl_fprint(fp, s->decl, 0); }
		   else if (s->init_expr) { expr_fprint(fp, s->init_expr); fprintf(fp, ";"); }
		   fprintf(fp, " ");
		   expr_fprint(fp, s->expr); fprintf(fp, "; "); // ; expr;
		   expr_fprint(fp, s->next_expr); fprintf(fp, ") "); // ; next_expr)
                   if (s->body && s->body->kind == STMT_BLOCK) stmt_fprint(fp, s->body, indent); // { body }
                   else {
		     fprintf(fp, "{\n"); stmt_fprint(fp, s->body, indent+1);
		     print_indent(fp, indent); fprintf(fp, "}");
		   }
		   break;

    // print; print expr; print expr, expr;
    case STMT_PRINT: fprintf(fp, "print"); if (s->expr) fprintf(fp, " ");
		     expr_fprint(fp, s->expr); fprintf(fp, ";");
		     break;

    // return; return expr; return expr, expr;
    case STMT_RETURN: fprintf(fp, "return"); if (s->expr) fprintf(fp, " ");
		      expr_fprint(fp, s->expr); fprintf(fp, ";");
		      break;
    /*
    {
      body
    }

    {}
    */
    case STMT_BLOCK: fprintf(fp, "{");
		     if (s->body) { fprintf(fp, "\n"); stmt_fprint(fp, s->body, indent+1); }
		     print_indent(fp, indent); fprintf(fp, "}");
		     if (s->next) { fprintf(fp, "\n"); }
		     break;
    /*
     while (expr) {
       body;
     }
    */
    case STMT_WHILE: fprintf(fp, "while ("); expr_fprint(fp, s->expr); fprintf(fp, ") ");
		     if (s->body && s->body->kind == STMT_BLOCK) stmt_fprint(fp, s->body, 0); // { body }
		     else {
		       fprintf(fp, "{\n"); stmt_fprint(fp, s->body, indent+1);
		       print_indent(fp, indent); fprintf(fp, "}");
		     }
		     break;
  }
  if (s->kind != STMT_BLOCK) fprintf(fp, "\n");
  stmt_fprint(fp, s->next, indent);
}
void stmt_print(struct stmt* s, int indent) { stmt_fprint(stdout, s, indent); }
