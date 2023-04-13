
COMP 4060 201
Assignment 3 - Pretty Printer
*****************************************************************************************************************************
Completion Rate: 99% (dynamically allocated memory not freed, though everything works)
*****************************************************************************************************************************

FILES (there are a lot of files, half of them are just .bminor test files)
5 directories, 67 files

Directories:
	source: holds the source code for the validator and scanner
	tests: holds ALL the files for running tests
	parser: subdirectory in tests to hold parser specific test files
	scanner: subdirectory in tests to hold scanner specific test files
	ast: subdirectory in tests to hold abstract syntax tree (ast) test files

*ALL .bminor files are test files*
.
├── Makefile : builds everything
├── README.txt
├── parse : validator executable
├── print : pretty printer executable
├── scan : scanner executable
├── source
│   ├── decl.c : declaration structure implementation file
│   ├── decl.h : declaration structure header file
│   ├── expr.c : expression structure implementation file
│   ├── expr.h : expression structure header file
│   ├── grammar.bison : grammar specification file
│   ├── grammar.txt : description of grammar (generated)
│   ├── main.c : runs the pretty printer
│   ├── param_list.c : parameter list implementation file
│   ├── param_list.h : parameter list header file
│   ├── parse.c : runs the validator
│   ├── parser.c : grammar automaton code (generated)
│   ├── parser.h : header file for tokens and ast structures (generated)
│   ├── scan.c : runs the scanner
│   ├── scanner.c : scanner automaton code (generated)
│   ├── scanner.flex : scanner specification file
│   ├── stmt.c : statement structure implementation file
│   ├── stmt.h : statement structure header file
│   ├── symbol.c : symbol structure implementation file
│   ├── symbol.h : symbol structure headerfile
│   ├── type.c : type structure implementaiton file
│   └── type.h : type structure header file
├── test : executable to run all tests
└── tests
    ├── ast
    │   ├── code.bminor
    │   ├── code_pretty.bminor : expected output file
    │   ├── decl_init.bminor
    │   ├── decl_uninit.bminor
    │   ├── expr_associativity.bminor
    │   ├── expr_postfix.bminor
    │   ├── expr_precedence.bminor
    │   ├── expr_vanilla.bminor
    │   ├── stmt_body.bminor
    │   ├── stmt_for.bminor
    │   ├── test_ast.c : runs ast tests
    │   ├── test_factory.c : runs factory function tests
    │   ├── test_print.c : runs print tests
    │   └── type_param_list.bminor
    ├── parser
    │   ├── code.bminor
    │   ├── declaration.bminor
    │   ├── empty.bminor
    │   ├── expression.bminor
    │   ├── function.bminor
    │   ├── initialization.bminor
    │   ├── iteration.bminor
    │   ├── jump.bminor
    │   ├── print.bminor
    │   ├── selection.bminor
    │   └── test_parser.c
    ├── scanner
    │   ├── character.bminor
    │   ├── code.bminor
    │   ├── foo.bminor
    │   ├── identifier.bminor
    │   ├── integer.bminor
    │   ├── keywords.bminor
    │   ├── string.bminor
    │   ├── string_format.bminor
    │   └── test_scanner.c
    ├── test.c : runs test executables
    ├── test_ast
    ├── test_factory
    ├── test_parse
    ├── test_print
    └── test_scan

Like before, all the files from the previous assignment(s) are included to ensure that ALL tests pass. This was really important
as the completion of this assigned required me to make changes to what I had written before. This was especially true in the grammar,
where incorrect rules could lead to associativity and precedence errors.

That being said, the following are some of the changes made:

Grammar:
  - Auto keyword is now part of type in type declarations
  - exp_expr is now right associative (it was left associative before)
  - Subscripts and function calls are now on the same precdence level as other unary expressions
    * how it is supposed to be/how it is in C
    * this change actually fixed A LOT of problems I had (such as proper nesting/listing. x[i](x, y), x[i][j][k], f(x)(y) was invalid)
  - Added an additional precedence level for initialization expressions: init_expr
    * Lower precedence than comma_expr/expr
    * unary operator
    * { expr }
    * also fixed other problems I had (such as proper nesting, listing, etc.)

Ast structures:
  - Additional expression types were added outside of those described by operator tokens from scanner:
    * EXPR_COMMA for expression lists
    * EXPR_INIT for initialization lists
    * EXPR_FCALL for function calls
    * EXPR_SUBSCRIPT for array subscripts
  - Operator tokens with multiple uses have multiple expression types:
    * TOKEN_ADD can be part of an expression being:
	- binary addition -> EXPR_ADD
	- unary addition/plus -> EXPR_POS
    * TOKEN_SUB can be part of an expression being:
	- binary subtraction -> EXPR_SUB
	- unary negation -> EXPR_NEG

Tests:
  - Some parser tests that were originally programs became test programs since they were only a line

Miscellaneous changes that do not affect anything:
  - main.c from the validator was renamed [parse.c]
  - [test_all] Makefile command renamed to make [tests]
  - Makefile command [compiler] added

***********************************************************************************
EXECUTABLES:
Also note on Makefile commands:
[make]/[make all] : compiles everything
[make compiler] : compiles source code targets
[make tests] : compiles test code targets

[print] : Runs the pretty printer
	  Compiled by [make], [make all], [make compiler] or [make print]

          Has -t option just like in [parse], since this main program is just a continuation of the validator
	  Can either print ASTs for a test_program or program.

	  Printing a test_program AST is done after each successful parse, whereas printing a program AST
	  is done once EOF is reached (which can be imitated with ^D if input is stdin).
	  
[parse] : Runs the validator
	  Compiled by [make], [make all], [make compiler] or [make parse]
	  
	  [parse] has 1 optional command line argument: -t
	  If [parse] is run with the -t option, it makes the value in yydebug be 1, showing all the
	  states and transitions the automaton goes through as output.

	  [parse] stops running the validator once EOF is reached if given a filename through input redirection.
	  If the input is stdin, use ^D (or ^C to stop at once).

[scan] : Runs the scanner
         Compiled by [make], [make all], [make compiler] or [make scan]

[test]  : Runs all the test executables
	  Compiled by [make], [make all], [make tests] or [make tests/test]

	  Displays total tests passed and run after all executables are run.
	
[test_ast] : Runs AST tests
	     Compiled by [make], [make all], [make tests] or [make tests/test_ast]

[test_print] : Runs print function tests
	       Compiled by [make], [make all], [make tests] or [make tests/test_print]

[test_factory] : Runs factory function tests
		 Compiled by [make], [make all], [make tests] or [make tests/test_factory]

[test_parse] : Runs parser tests
	       Compiled by [make], [make all], [make tests] or [make tests/test_parse]

[test_scan] : Runs scanner tests
	      Compiled by [make], [make all], [make tests] or [make tests/test_scan]

***********************************************************************************
TESTS:
There are 3 test executables for AST testing. Tests were done in these three components since
each part relied on the previous. Print functions require AST structures, only available from the factory functions.
And pretty printing of (entire) programs requires fully working print funcitons.

This approach also ensured that any problems occuring during testing of the current component is that it is more
likely to be due to the current component and not what it depends on.

  - test_factory
        * Tests the factory functions to see if they are storing everything correctly.
        * Integration tests made the code get REALLY messy (It's pointer hell spaghetti)
        * Uses manually created AST structures
  - test_print
        * Tests the print functions to see if they print 'prettily'/does the formatting correctly
        * Uses manually created AST structures
  - test_ast
        * Tests how well bison constructs the ASTs from the grammar rules (automatically created)
        * Focuses on grammar specific AST construction: precedence, associatvity, etc.
        * Uses .bminor files for input either programs or test programs

**********************************************************************************
PRETTY PRINTING:

What is deemed as 'pretty' is subjective, so the following is a list of things does for
structure formatting to make it be 'pretty':

1. All statement bodies start and end with curly brackets.
   So something like for( ; ; ) foo(); instead becomes:
   
   for ( ; ; ) {
     foo();
   }

   The one exception to this is block statements, which already have the curly brackets as part of its statement print.
   Also EMPTY block statements (which can be statement bodies) DO NOT have newlines between the brackets.

   The 'prettily printed' body is just this: {}

2. All operands and operators are space seperated, with some exceptions:
   - unary operators: !, +, -, ++, --, {}
   - 'wrap' operators: [], ()
   - comma operator only includes space before printing right subtree:

	this is pretty: x, y
	this is not: x , y

3. Parentheses are added around an expression subtree if it is not of a primitive type.
   These expressions do not get parentheses: (), [], {}, comma

4. Keywords (if, for, function, void, etc.) are always followed by a space
   (Unless it is an uninitialized declaration, where the keyword is not followed by a space, just a semicolon ;)

5. Expressions and parameter lists within parentheses do not have spaces between the left parentheses and the first value and the right
parentheses with the last value:

   This is a pretty parameter list: (x: integer, y: char, c: boolean, d: string)

6. All declarations (includes parameters) have a space following the :

7. Function declarations that lack a parameter list instead have it filled with 'void'.

**************************************************************************************************
MISC

1. all <struct>_print functions have new function <struct>_fprint, that prints to a given file stream:

  void expr_fprint(FILE* fp, struct expr* e);
  void type_fprint(FILE* fp, struct type* t);
  void param_list_fprint(FILE* fp, struct param_list* p);
  void decl_fprint(FILE* fp, struct decl* d, int indent);
  void stmt_fprint(FILE* fp, struc stmt* s, int ident);

  the original <struct>_print function from the starter code is just a special case of this function, where
  the file pointer is stdout (just like how printf(...) is just fprintf(stdout, ...))

  ex) expr_print:
  void expr_print(struct expr* e) { expr_fprint(stdout, e); }

  This feature was added in order to test the print functions prior to bison integration, so that the
  output could be saved somewhere (attempts were done with a pipe, but no avail/debugging I did not want to go through)

  type also has an additional function: type_subtype_leaf_assign(struct type* t, struct type* subtype)
  which assigns a subtype to the LEAF of the type tree with t as the root of it. This was helpful in the
  grammar in assigning the primitive type of a compositie type.

2. struct type now has additional field to hold its size when in declarations:
        struct type {
                int kind;
                struct type* subtype;
                struct param_list* params;
                struct expr* size;
        };

        This is used in type_print so that array types are properly printed with their size.
        Also in cases of nested arrays:
        
        ex) array [3] array [30 - 2] integer, the associated sizes remain with their subtypes.

        This also means type_create gets an updated signature.

3. While statements are implemented

4. AST structures do not have proper destroy() functions (yet), meaning the dynamically allocated memory
   is not being freed.

5. There are 3 shift-reduce conflicts:
	State 78 conflicts: 1 shift/reduce -> rules name and primary_expr
	State 154 conflicts: 1 shift/reduce -> rules init and expr
	State 186 conflicts: 1 shift/reduce -> dangling else

***********************************************************************************************
DIFFICULTIES

1. Integrating the semantic types with the parser.
   
   It was difficult because I did NOT #include all the necessary files and LINK them with any executable/.c file
   that used the parser/semantic types after implementing and testing them.
   (that is, linker was yelling at me because test_parse executable had undefined references to factory
    functions because it was now included in the grammar, which was not the case when the parser and its test
    code was only a validator)

   Several hours were wasted figuring it out :/.
   When I figured this out it was VERY smooth sailing.

2. Not realizing that strdup exists.
   Before I used it the name rule name for expr_create_name and just anywhere theat used name was not being stored properly.
   It gave funny things like:
   
   x+y:
   x + y; + y;

   x[i][j]
   x[i][j];[i][j];];

   With MANUALLY created ast structures, this isn't a problem. So it was a problem with bison and pointers.
   I eventually realized that it was because when using expr_create_name and so forth it was not saving its
   own copy of the name.
   After using strdup everything worked very well.
