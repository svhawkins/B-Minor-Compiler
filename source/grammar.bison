%{
  #include <stdio.h>
  #include <stdlib.h>
  #include <string.h>
  #define YYLMAX 256
  extern char* yytext;
  extern int yylex();
  extern int yyerror(const char* str);
  char error_text[YYLMAX];
%}
%define parse.error verbose

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

%%

stmt: expr_stmt TOKEN_SEMI { return 0; }

expr_stmt : expr
	  ;

expr : assign_expr
     | expr TOKEN_COMMA assign_expr
     ;

assign_expr : lor_expr
     | unary_expr TOKEN_ASSIGN assign_expr
     ;

unary_expr : postfix_expr
	   | TOKEN_ADD cast_expr
	   | TOKEN_NOT cast_expr
	   | TOKEN_SUB cast_expr
	   ;
	
cast_expr : unary_expr;

postfix_expr : primary_expr
	     | postfix_expr TOKEN_LBRACK expr TOKEN_RBRACK
	     | postfix_expr TOKEN_INC
	     | postfix_expr TOKEN_DEC
	     ;

primary_expr : TOKEN_BOOL
	     | TOKEN_CH
	     | TOKEN_IDENT
	     | TOKEN_NUMBER
	     | TOKEN_STR
	     | TOKEN_LPAR expr TOKEN_RPAR
	     ;

lor_expr : land_expr
	 | lor_expr TOKEN_OR land_expr
	 ;

land_expr : eq_expr
	  | land_expr TOKEN_AND eq_expr
	  ;

eq_expr : rel_expr
	| eq_expr TOKEN_EQ rel_expr
	| eq_expr TOKEN_NEQ rel_expr
	;

rel_expr : add_expr
	 | rel_expr TOKEN_LESS add_expr
	 | rel_expr TOKEN_GREAT add_expr
	 | rel_expr TOKEN_LEQ add_expr
	 | rel_expr TOKEN_GEQ add_expr
	 ;

add_expr : mult_expr
	 | add_expr TOKEN_ADD mult_expr
	 | add_expr TOKEN_SUB mult_expr
	 ;

mult_expr : exp_expr
	  | mult_expr TOKEN_MULT exp_expr
	  | mult_expr TOKEN_DIV exp_expr
	  | mult_expr TOKEN_MOD exp_expr
	  ;

exp_expr : cast_expr
	 | mult_expr TOKEN_EXP cast_expr
	 ;

%%
int yyerror(const char* str) {
  sprintf(error_text, "%s", str);
  return 0;
}
