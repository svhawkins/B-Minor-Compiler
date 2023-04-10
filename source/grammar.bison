%{
  #include <stdio.h>
  #include <stdlib.h>
  #include <string.h>
  #include <stdbool.h>
  #include <ctype.h>
  #include "decl.h"
  #include "expr.h"
  #include "param_list.h"
  #include "stmt.h"
  #include "symbol.h"
  #include "type.h"
  #define YYLMAX 256
  extern char* yytext;
  extern int yylex();
  extern int yyerror(const char* str);
  char* name_clean(char* s); // cleans yytext. 
  char error_text[YYLMAX];
  unsigned char eof = 0;
  bool bool_convert(char* s); /* converts given TOKEN_BOOL string to proper integer boolean value */
  void print_error_message(void);



  // parser results
  struct stmt* test_parser_result = NULL; // for testing
  struct decl* parser_result = NULL;
  
%}

%define parse.error verbose

/* tokens */
%token TOKEN_ERROR
%token TOKEN_EOF

%token TOKEN_ARRAY
%token TOKEN_AUTO
%token TOKEN_BOOLEAN
%token TOKEN_CHAR
%token TOKEN_ELSE
%token TOKEN_FOR
%token TOKEN_FUNCTION
%token TOKEN_IF
%token TOKEN_INTEGER
%token TOKEN_PRINT
%token TOKEN_RETURN
%token TOKEN_STRING
%token TOKEN_VOID
%token TOKEN_WHILE

%token TOKEN_IDENT
%token TOKEN_BOOL
%token TOKEN_NUMBER
%token TOKEN_STR
%token TOKEN_CH

%token TOKEN_INC
%token TOKEN_DEC
%token TOKEN_SUB
%token TOKEN_EXP
%token TOKEN_NOT
%token TOKEN_MULT
%token TOKEN_DIV
%token TOKEN_MOD
%token TOKEN_ADD
%token TOKEN_LEQ
%token TOKEN_LESS
%token TOKEN_GEQ
%token TOKEN_GREAT
%token TOKEN_EQ
%token TOKEN_NEQ
%token TOKEN_AND
%token TOKEN_OR
%token TOKEN_ASSIGN

%token TOKEN_LBRACK
%token TOKEN_RBRACK
%token TOKEN_LPAR
%token TOKEN_RPAR
%token TOKEN_LCURL
%token TOKEN_RCURL
%token TOKEN_COLON
%token TOKEN_SEMI
%token TOKEN_COMMA

/* semantic value types */
%union {
  struct expr* expr;
  struct stmt* stmt;
  struct decl* decl;
  struct type* type;
  struct param_list* param_list;
  char* name;
}
%nterm <name> name
%type <expr> primitive primary_expr subscript expr postfix_expr unary_expr assign_expr lor_expr land_expr eq_expr rel_expr add_expr mult_expr exp_expr
%type <stmt> expr_stmt stmt test_program program
//%type <decl> ext_decl function_decl

%%
program : ext_decl { return 0; }
	| program ext_decl
	| TOKEN_MOD TOKEN_MOD test_program { test_parser_result = $3; return 0; }
	| TOKEN_EOF { parser_result = NULL; test_parser_result = NULL; eof = 1; return 0; }
	;

test_program : stmt { $$ = $1; }
	     | ext_decl
	     ;

decl : name TOKEN_COLON type TOKEN_SEMI
     | name TOKEN_COLON type TOKEN_ASSIGN init TOKEN_SEMI
     ;

init : TOKEN_LCURL init_list TOKEN_RCURL
     | expr
     ;

init_list : init
          | init_list TOKEN_COMMA init
          ;
 
name : TOKEN_IDENT { $$ = yytext; }
     | TOKEN_LPAR name TOKEN_RPAR { $$ = $2; }
     ;

ext_decl : decl
         | function_decl
         ;

stmt : print_stmt
     | expr_stmt { $$ = $1; }
     | iter_stmt
     | jump_stmt
     | block_stmt
     | select_stmt
     ;

print_stmt : TOKEN_PRINT expr TOKEN_SEMI
	   ;

expr_stmt : expr TOKEN_SEMI { $$ = stmt_create(STMT_EXPR, NULL, NULL, $1, NULL, NULL, NULL, NULL); } 
	  ;

expr : assign_expr { $$ = $1; }
     | expr TOKEN_COMMA assign_expr { $$ = expr_create(EXPR_COMMA, $1, $3); } 
     ;

assign_expr : lor_expr { $$ = $1; }
            | unary_expr TOKEN_ASSIGN assign_expr { $$ = expr_create(EXPR_ASSIGN, $1, $3); }
            ;

unary_expr : postfix_expr { $$ = $1; }
	   | TOKEN_ADD unary_expr { $$ = expr_create(EXPR_POS, $2, NULL); }
	   | TOKEN_NOT unary_expr { $$ = expr_create(EXPR_NOT, $2, NULL); }
	   | TOKEN_SUB unary_expr { $$ = expr_create(EXPR_NEG, $2, NULL); }
	   ;

postfix_expr : primary_expr { $$ = $1; }
	     | postfix_expr TOKEN_INC { $$ = expr_create(EXPR_INC, $1, NULL); }
	     | postfix_expr TOKEN_DEC { $$ = expr_create(EXPR_DEC, $1, NULL); }
	     ;

primary_expr : primitive { $$ = $1; }
	     | TOKEN_LPAR expr TOKEN_RPAR { $$ = $2; }
	     | name TOKEN_LPAR TOKEN_RPAR  { $$ = expr_create(EXPR_FCALL, expr_create_name(name_clean($1)), NULL); }
	     | name TOKEN_LPAR expr TOKEN_RPAR { $$ = expr_create(EXPR_FCALL, expr_create_name(name_clean($1)), $3); }
	     | name { $$ = expr_create_name(name_clean($1)); }
	     | name subscript { $2->left = expr_create_name(name_clean($1)); $$ = $2; }
	     | name subscript subscript
	       { $2->left = expr_create_name(name_clean($1)); $3->left = $2; $$ = $3; }
	     | name subscript subscript subscript
	       { $2->left = expr_create_name(name_clean($1)); $3->left = $2; $4->left = $3; $$ = $4; }
	     | name subscript subscript subscript subscript
	       { $2->left = expr_create_name(name_clean($1)); $3->left = $2; $4->left = $3; $5->left = $4; $$ = $5; }
	     | name subscript subscript subscript subscript subscript
	       { $2->left = expr_create_name(name_clean($1)); $3->left = $2; $4->left = $3; $5->left = $4; $6->left = $5; $$ = $6; }
	     ;

subscript : TOKEN_LBRACK assign_expr TOKEN_RBRACK { $$ = expr_create(EXPR_SUBSCRIPT, NULL, $2); }
	  ;

primitive : TOKEN_BOOL { $$ = expr_create_boolean_literal(bool_convert(yytext)); }
	  | TOKEN_CH { $$ = expr_create_char_literal(yytext[0]); }
          | TOKEN_NUMBER { $$ = expr_create_integer_literal(atoi(yytext)); }
	  | TOKEN_STR { $$ = expr_create_string_literal(yytext); }
	  ;

lor_expr : land_expr { $$ = $1; }
	 | lor_expr TOKEN_OR land_expr { $$ = expr_create(EXPR_OR, $1, $3); }
	 ;

land_expr : eq_expr { $$ = $1; }
	  | land_expr TOKEN_AND eq_expr { $$ = expr_create(EXPR_AND, $1, $3); }
	  ;

eq_expr : rel_expr { $$ = $1; }
	| eq_expr TOKEN_EQ rel_expr { $$ = expr_create(EXPR_EQ, $1, $3); }
	| eq_expr TOKEN_NEQ rel_expr { $$ = expr_create(EXPR_NEQ, $1, $3); }
	;

rel_expr : add_expr { $$ = $1; }
	 | rel_expr TOKEN_LESS add_expr { $$ = expr_create(EXPR_LESS, $1, $3); }
	 | rel_expr TOKEN_GREAT add_expr { $$ = expr_create(EXPR_GREAT, $1, $3); }
	 | rel_expr TOKEN_LEQ add_expr { $$ = expr_create(EXPR_LEQ, $1, $3); }
	 | rel_expr TOKEN_GEQ add_expr { $$ = expr_create(EXPR_GEQ, $1, $3); }
	 ;

add_expr : mult_expr { $$ = $1; }
	 | add_expr TOKEN_ADD mult_expr { $$ = expr_create(EXPR_ADD, $1, $3); }
	 | add_expr TOKEN_SUB mult_expr { $$ = expr_create(EXPR_SUB, $1, $3); }
	 ;

mult_expr : exp_expr { $$ = $1; }
	  | mult_expr TOKEN_MULT exp_expr { $$ = expr_create(EXPR_MULT, $1, $3); }
	  | mult_expr TOKEN_DIV exp_expr { $$ = expr_create(EXPR_DIV, $1, $3); }
	  | mult_expr TOKEN_MOD exp_expr { $$ = expr_create(EXPR_MOD, $1, $3); }
	  ;

exp_expr : unary_expr { $$ = $1; }
	 | unary_expr TOKEN_EXP exp_expr { $$ = expr_create(EXPR_EXP, $1, $3); }
	 ;

select_stmt  : TOKEN_IF TOKEN_LPAR expr TOKEN_RPAR stmt
	     | TOKEN_IF TOKEN_LPAR expr TOKEN_RPAR stmt TOKEN_ELSE stmt
	     ;

iter_stmt : TOKEN_WHILE TOKEN_LPAR expr TOKEN_RPAR stmt
          | TOKEN_FOR TOKEN_LPAR TOKEN_SEMI TOKEN_SEMI TOKEN_RPAR stmt
          | TOKEN_FOR TOKEN_LPAR TOKEN_SEMI TOKEN_SEMI expr TOKEN_RPAR stmt
          | TOKEN_FOR TOKEN_LPAR TOKEN_SEMI expr_stmt TOKEN_RPAR stmt
          | TOKEN_FOR TOKEN_LPAR TOKEN_SEMI expr_stmt expr TOKEN_RPAR stmt
          | TOKEN_FOR TOKEN_LPAR expr_stmt TOKEN_SEMI TOKEN_RPAR stmt
          | TOKEN_FOR TOKEN_LPAR expr_stmt TOKEN_SEMI expr TOKEN_RPAR stmt
          | TOKEN_FOR TOKEN_LPAR expr_stmt expr_stmt TOKEN_RPAR stmt
          | TOKEN_FOR TOKEN_LPAR expr_stmt expr_stmt expr TOKEN_RPAR stmt
          | TOKEN_FOR TOKEN_LPAR decl TOKEN_SEMI TOKEN_RPAR stmt
          | TOKEN_FOR TOKEN_LPAR decl TOKEN_SEMI expr TOKEN_RPAR stmt
          | TOKEN_FOR TOKEN_LPAR decl expr_stmt TOKEN_RPAR stmt
          | TOKEN_FOR TOKEN_LPAR decl expr_stmt expr TOKEN_RPAR stmt
          ;

jump_stmt : TOKEN_RETURN TOKEN_SEMI
	  | TOKEN_RETURN expr TOKEN_SEMI
	  ;

block_stmt : TOKEN_LCURL TOKEN_RCURL
           | TOKEN_LCURL block_list TOKEN_RCURL
           ;

block_list : block
           | block_list block
           ;

block : decl
      | stmt
      ;


function_decl : name TOKEN_COLON ret_type TOKEN_LPAR param_list TOKEN_RPAR TOKEN_SEMI
	      | name TOKEN_COLON ret_type TOKEN_LPAR param_list TOKEN_RPAR TOKEN_ASSIGN block_stmt
	      ;

param_list : params
	   | TOKEN_VOID
	   | %empty
	   ;

params : param TOKEN_COMMA params
       | param
       ;

param : name TOKEN_COLON param_type
      ;

param_type : type
	   | function_type
	   ;

type : primitive_type
     | array_list array_type
     | TOKEN_AUTO
     ;

function_type : TOKEN_FUNCTION ret_type
	      ;

array_type : primitive_type
	   | function_type
	   ;

primitive_type : TOKEN_BOOLEAN
	    | TOKEN_CHAR
	    | TOKEN_INTEGER
	    | TOKEN_STRING
	    ;

ret_type : primitive_type
	 | TOKEN_VOID
	 | array_list array_type
	 | function_type
	 ;

array_list : array
	   | array_list array
	   ;

array : TOKEN_ARRAY TOKEN_LBRACK assign_expr TOKEN_RBRACK
      | TOKEN_ARRAY TOKEN_LBRACK TOKEN_RBRACK
      ;

%%
int yyerror(const char* str) {
  fflush(stdout);
  if (!eof) { sprintf(error_text, "%s", str); eof = 0; }
  return 0;
}

/* prints error messages that are NOT eof */
void print_error_message(void) { fprintf(stderr, "%s\n", error_text); }

bool bool_convert(char* s) { return (!strcmp(s, "true")); }

char* name_clean(char* s) {
  int i, j; char new_str[strlen(s)];
  for (i = 0; isspace(s[i]); i++) {} // skip whitespace
  for (j = 0; isalnum(s[i]) || s[i] == '_'; i++, j++) new_str[j] = s[i];
  new_str[j] = '\0';
  strcpy(s, new_str);
  return s;
}
  
