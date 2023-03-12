%{
  #include <stdio.h>
  #include <stdlib.h>
  #include <string.h>
  #define YYLMAX 256
  extern char* yytext;
  extern int yylex();
  extern int yyerror(const char* str);
  char error_text[YYLMAX];
  unsigned char eof = 0;
  void print_error_message(void);
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
program : ext_decln { return 0; }
	| program ext_decln
	| TOKEN_MOD TOKEN_MOD test_program { return 0; }
	| TOKEN_EOF { eof = 1; return 0; }
	;

test_program : stmt
	     | ext_decln
	     ;

decl : decltr TOKEN_COLON type
     ;

ext_decln : decln
          | function_decln
          ;

stmt : print_stmt
     | expr_stmt
     | iter_stmt
     | jump_stmt
     | cmpnd_stmt
     | select_stmt
     ;

print_stmt : TOKEN_PRINT print_list TOKEN_SEMI
	   ;

print_list : assign_expr TOKEN_COMMA print_list
	   | assign_expr
	   ;

expr_stmt : expr TOKEN_SEMI
	  ;

expr : assign_expr
     | expr TOKEN_COMMA assign_expr
     ;

assign_expr : lor_expr
     | unary_expr TOKEN_ASSIGN assign_expr
     ;

unary_expr : postfix_expr
	   | TOKEN_ADD unary_expr
	   | TOKEN_NOT unary_expr
	   | TOKEN_SUB unary_expr
	   ;

postfix_expr : primary_expr
	     | postfix_expr TOKEN_LPAR expr TOKEN_RPAR
	     | postfix_expr TOKEN_INC
	     | postfix_expr TOKEN_DEC
	     ;

primary_expr : primitive
	     | TOKEN_LPAR expr TOKEN_RPAR
	     | lvalue
	     ;

primitive : TOKEN_BOOL | TOKEN_CH | TOKEN_NUMBER | TOKEN_STR ;

expr_list : expr 
	  | expr_list TOKEN_COMMA expr
	  ;

lvalue : decltr suffix ;
suffix : call_suffix | subscript_list | %empty ;
designator : decltr subscript_list ;
call_suffix : TOKEN_LPAR TOKEN_RPAR | TOKEN_LPAR expr_list TOKEN_RPAR ;

subscript_list : subscript
	       | subscript_list subscript
	       ;

subscript : TOKEN_LBRACK assign_expr TOKEN_RBRACK
	  ;

decltr : TOKEN_IDENT
       | TOKEN_LPAR decltr TOKEN_RPAR
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

exp_expr : unary_expr
	 | exp_expr TOKEN_EXP unary_expr
	 ;

select_stmt  : TOKEN_IF TOKEN_LPAR expr TOKEN_RPAR stmt
	     | TOKEN_IF TOKEN_LPAR expr TOKEN_RPAR stmt TOKEN_ELSE stmt
	     ;

iter_stmt : TOKEN_WHILE TOKEN_LPAR expr TOKEN_RPAR stmt
	  | TOKEN_FOR TOKEN_LPAR for TOKEN_RPAR stmt
	  ;

for : TOKEN_SEMI TOKEN_SEMI
    | TOKEN_SEMI TOKEN_SEMI expr
    | TOKEN_SEMI expr_stmt
    | TOKEN_SEMI expr_stmt expr
    | expr_stmt TOKEN_SEMI
    | expr_stmt TOKEN_SEMI expr
    | expr_stmt expr_stmt
    | expr_stmt expr_stmt expr
    | decln TOKEN_SEMI
    | decln TOKEN_SEMI expr
    | decln expr_stmt
    | decln expr_stmt expr
    ;


jump_stmt : TOKEN_RETURN TOKEN_SEMI
	  | TOKEN_RETURN expr TOKEN_SEMI
	  ;

cmpnd_stmt : TOKEN_LCURL TOKEN_RCURL
           | TOKEN_LCURL block_list TOKEN_RCURL
           ;

block_list : block
           | block_list block
           ;

block : int_decln
      | stmt
      ;

int_decln : decln | lvalue initialization ;

initialization : TOKEN_ASSIGN init TOKEN_SEMI ;

decln : decl TOKEN_SEMI
      | decl initialization
      ;


function_decl : decltr TOKEN_COLON TOKEN_FUNCTION ret_type
	      ;

function_suffix : TOKEN_SEMI
		| TOKEN_ASSIGN cmpnd_stmt
		;

function_decln : function_decl TOKEN_LPAR TOKEN_RPAR function_suffix
	       | function_decl TOKEN_LPAR TOKEN_VOID TOKEN_RPAR function_suffix
	       | function_decl TOKEN_LPAR param_list TOKEN_RPAR function_suffix
	       ;

param_list : decl_list
	   ;

decl_list : decl TOKEN_COMMA decl_list
	  | decl
	  ;

init : TOKEN_LCURL init_list TOKEN_RCURL
     | assign_expr
     ;

init_list : init
	  | init_list TOKEN_COMMA init
	  ;


type : atomic_type
     | TOKEN_AUTO
     | array_list atomic_type
     ;

atomic_type : TOKEN_BOOLEAN
	    | TOKEN_CHAR
	    | TOKEN_INTEGER
	    | TOKEN_STRING
	    ;

ret_type : atomic_type
	 | TOKEN_VOID
	 | array_list atomic_type
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
