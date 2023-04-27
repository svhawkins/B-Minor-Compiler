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

void stmt_destroy(struct stmt** s) {
  if (!(*s)) return;
  decl_destroy(&((*s)->decl));
  expr_destroy(&((*s)->init_expr));
  expr_destroy(&((*s)->expr));
  expr_destroy(&((*s)->next_expr));
  stmt_destroy(&((*s)->body));
  stmt_destroy(&((*s)->else_body));
  stmt_destroy(&((*s)->next));
  free(*s); *s = NULL;
}

int stmt_resolve(struct symbol_table* st, struct stmt* s) {
  if (!st || !s) return 0; int error_status = 0;
  switch (s->kind) {

    case STMT_DECL: error_status = decl_resolve(st, s->decl); break;
    case STMT_EXPR: error_status = expr_resolve(st, s->expr); break;

    // evaluate only the expression
    case STMT_PRINT: error_status = expr_resolve(st, s->expr); break;
    case STMT_RETURN: error_status = expr_resolve(st, s->expr); break;

    // introduces new scope(s)
    case STMT_BLOCK:
      symbol_table_scope_enter(st);
      error_status = stmt_resolve(st, s->body);
      symbol_table_scope_exit(st);
      break;
    case STMT_WHILE:
      error_status = expr_resolve(st, s->expr);
      if (s->body && s->body->kind != STMT_BLOCK) symbol_table_scope_enter(st);
      error_status = stmt_resolve(st, s->body);
      if (s->body && s->body->kind != STMT_BLOCK) symbol_table_scope_exit(st);
      break;
    case STMT_IF_ELSE:
      error_status = expr_resolve(st, s->expr);
      if (s->body && s->body->kind != STMT_BLOCK) symbol_table_scope_enter(st);
      error_status = stmt_resolve(st, s->body);
      if (s->body && s->body->kind != STMT_BLOCK) symbol_table_scope_exit(st);
      if (s->else_body) {
        if (s->else_body && s->else_body->kind != STMT_BLOCK) symbol_table_scope_enter(st);
        error_status = stmt_resolve(st, s->else_body);
        if (s->else_body && s->else_body->kind != STMT_BLOCK) symbol_table_scope_exit(st);
      } break;
    case STMT_FOR:
      // for statements can have declarations or just expressions
      error_status = (s->init_expr) ? expr_resolve(st, s->init_expr) : decl_resolve(st, s->decl);
      error_status = expr_resolve(st, s->expr);
      error_status = expr_resolve(st, s->next_expr);
      if (s->body && s->body->kind != STMT_BLOCK) symbol_table_scope_enter(st);
      error_status = stmt_resolve(st, s->body);
      if (s->body && s->body->kind != STMT_BLOCK) symbol_table_scope_exit(st);
      break;
  }
  return stmt_resolve(st, s->next);
}

int stmt_typecheck(struct symbol_table* st, struct stmt* s) {
  if (!s) return 0; int error_status = 0;
  struct type* t = NULL;
  switch(s->kind) {
    case STMT_EXPR: t = expr_typecheck(st, s->expr); type_destroy(&t); break;
    case STMT_DECL: error_status = decl_typecheck(st, s->decl); break;
    case STMT_PRINT: t = expr_typecheck(st, s->expr); /* TO DO: check if not function/array/void */ type_destroy(&t); break;
    case STMT_RETURN: t = expr_typecheck(st, s->expr); /* TO DO: compare with return type */ type_destroy(&t); break;
    case STMT_BLOCK:  error_status = stmt_typecheck(st, s->body); break;
    case STMT_IF_ELSE:
      t = expr_typecheck(st, s->expr);
      if (t->kind != TYPE_BOOLEAN) { /* TO DO: error message */ }
      type_destroy(&t);
      error_status = stmt_typecheck(st, s->body); error_status = stmt_typecheck(st, s->else_body);
      break;
    case STMT_WHILE:
      t = expr_typecheck(st, s->expr);
      if (t->kind != TYPE_BOOLEAN) { /* TO DO: error message */ }
      type_destroy(&t);
      error_status = stmt_typecheck(st, s->body);
      break;
    case STMT_FOR:
      t = expr_typecheck(st, s->init_expr);
      t = expr_typecheck(st, s->expr);
      if (t && t->kind != TYPE_BOOLEAN) { /* TO DO: error message */ }
      t = expr_typecheck(st, s->next_expr); type_destroy(&t);
      error_status = stmt_typecheck(st, s->body);
      break;
  }
  return stmt_typecheck(st, s->next);
}
