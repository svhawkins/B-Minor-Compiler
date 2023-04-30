
*********************
Files
6 directories, 101 files*
*Not all files are shown. Test, old, or .bminor files are not shown in this file tree.

.
├── Makefile
├── README.txt
├── source
│   ├── decl.c
│   ├── decl.h
│   ├── expr.c
│   ├── expr.h
│   ├── grammar.bison
│   ├── hash_table.c
│   ├── hash_table.h
│   ├── main.c
│   ├── param_list.c
│   ├── param_list.h
│   ├── parse.c
│   ├── parser.c
│   ├── parser.h
│   ├── print.c
│   ├── scanner.c
│   ├── scanner.flex
│   ├── stack.c : implementation file for a 'generic' stack interface
│   ├── stack.h : header file for a 'generic' stack interface
│   ├── stmt.c
│   ├── stmt.h
│   ├── symbol.c
│   ├── symbol.h
│   ├── symbol_table.c : implementation file for symbol table interface
│   ├── symbol_table.h : header file for symbol table interface, using a 'generic' stack
│   ├── type.c
│   └── type.h
├── test
├── tests
│   ├── ast : directory for ast tests + code
│   ├── parser : directory for parser tests + code
│   ├── scanner : directory for scanner tests + code
│   ├── symbol_table
│   │   ├── test_stack.c : symbol table + name resolution tests
│   │   └── test_typecheck.c : typechecking tests
│   ├── test.c : runs all test executables
│   ├── test_ast
│   ├── test_factory
│   ├── test_parse
│   ├── test_print
│   ├── test_scan
│   ├── test_stack : runs name resolution + symbol table tests (includes structures that compose them)
│   └── test_typecheck : runs typechecking tests
└── typecheck

****************************************************************************************

EXECUTABLES
commands:
[all] - compiles all code targets under [compiler] and [tests]
[compiler] - compiles all source code targets:
		  [scan], [parse], [print], [typecheck]
[tests] - compiles all test code targets:
		  scanner: [tests/test_scan]
              parser: [tests/test_parse]
		  ast: [tests/test_factory], [tests/test_print], [tests/test_ast]
		  typechecking: [tests/test_stack], [tests/test_typecheck]

The new executable is [typecheck]

[typecheck]:
	compiled by [make], [make all], [make compiler], or [make typecheck]
	[typecheck] runs a typechecker, for either programs or test programs and prints
      out the resulting symbol table as well as any errors that occur when doing
      name resolution or typechecking. Printing the resulting table is done once EOF/^D is reached.

	[typecheck] has the following options: -c, -v

	-c indicates "clear". This is for test programs (attempts were done to make it
	work for regular programs but it didn't work).
        After EOF is reached -c makes it that the symbol table is to be cleared after each statement of input.
        Otherwise the symbol table is the same for all statements inputted.

	-v indicates "verbose". This prints out the entire symbol table stack, even when
	out of scope. It also indicates the current table in scope if it were to not be verbose.

	Using the following code as an example:
	foo: function integer(a: char) = {
		i: integer;
		return i;
	}

      Normal output:

		SCOPE [0]: CURRENT (TOP) GLOBAL (BOTTOM)
	--------------------------------------------------

	foo --> (kind: global, name: foo, type: function integer (a: char))

	--------------------------------------------------
	Total errors: 0


	Verbose output:

		SCOPE [2]:
	--------------------------------------------------

	i --> (kind: local, name: i, type: integer)

	--------------------------------------------------
	SCOPE [1]:
	--------------------------------------------------

	a --> (kind: parameter, name: a, type: char)

	--------------------------------------------------
	SCOPE [0]: CURRENT (TOP) GLOBAL (BOTTOM)
	--------------------------------------------------

	foo --> (kind: global, name: foo, type: function integer (a: char))

	--------------------------------------------------
	Total errors: 0

*********************************************************************************************
ERROR MESSAGES

This typechecker also has various error messages, grouped together based on the structures that trigger them:
	symbol_table, stmt, and expr.

These structures are used as prefixes for the error_handle() functions, which take the error kind and a various amount
of void pointer parameters, acting as contexts.
All of them return the passed in error kind as an error code.

int symbol_table_error_handle(symbol_error_t kind, void* ctx1, void* ctx2);
int stmt_error_handle(stmt_error_t kind, void* ctx1, void* ctx2);
int expr_error_handle(type_error_t kind, void* ctx1, void* type_ctx1, void* ctx2, void* type_ctx2);

All of these functions do is print the corresponding error message given the kind and context(s). Recovery may also be done.
They also all increment the global error counter and print to file pointer stderr.

Symbol table errors:
    - SYM_UNDEF (1): undefined symbol used (found in no scope). Recover by adding it to table with default type integer 
    - SYM_REDEF (2): symbol name is already being used in current scope
    - SYM_TYPE (3):  symbol type does not match its value/return type
    - SYM_PARAM (4): parameter types do not match definiton

Statement errors:
    - STMT_BOOLEAN (1): resulting expression type is not of boolean type
    - STMT_TYPE (2): invalid type for printing

Expression (type) errors:
    - MATH (1): operands must be integers
    - RELATE (2): operands must be integers
    - LOGIC (3): operands must be booleans
    - EQUAL (4): operands must be the same type, cannot be function/array
    - ASSIGN (5): operands must be the same type
    - LVAL (6): left operand cannot be a literal
    - INIT (7): elements within list must be the same type
    - SUBSCRIPT (8): left type must be array, right type must be integer
    - FCALL (9): left type must be a function
    - PARAM (10): argument types must match with parameter list types
	** this does NOT print the argument list

Improvements can obviously be made, such as passing an array of pointers, only using one function, better naming,
printing argument lists, etc., but that is for another time.
Another major shortcoming is that I did not have the time/motivation to test out ALL of the error messages
(only the expression type errors are tested really), so there
may be a chance that error(s) may incorrectly appear or not appear at all (maybe even a segfault...).

****************************************************************************************************************************************************
SYMBOL TABLE
The symbol table was implemented by using a 'generic' stack interface, where the hash tables to be used are cast as void pointers and vice versa.
The symbol table itself is slightly more than just a stack, as instead of just a mere typedef, it is a struct:

struct symbol_table {
  Stack* stack;
  bool verbose;
  int top; 
}; typedef struct symbol_table Symbol_table;

The stack field is the actual table, but there are two additional fields:
	- verbose, which holds the verbose setting (by default false, enabled with -v), 
	- top, which holds the current index of the topmost table.
Because of the verbose setting, the index stored in top is not always the top of the stack, so to keep everything working the same regardless
of verbose setting, top is used (it's also good to have when doing multiple tree traversals!)


The symbol table has the following functions available:
(Further description/implementation can be read in source/symbol_table.h and source/symbol_table.c (and perhaps source/stack.h and source/stack.c as well!))

Construction/Destruction:
- struct symbol_table* symbol_table_create()
	Creates a non-verbose symbol table
- struct symbol_table* symbol_table_verbose_create()
	Creates a verbose symbol table
- symbol_table_destroy(struct symbol_table** st)
	Destroys a symbol table
- struct symbol_table* symbol_table_clear(struct symbol_table* st)
	Clears a symbol table

Mutation:
- void symbol_table_scope_enter(struct symbol_table* st)
	Enters new scope
- void symbol_table_scope_exit(struct symbol_table* st) 
	Exits topmost scope
- int symbol_table_scope_bind(struct symbol_table* st, const char* name, struct symbol* sym)
	Adds key-value pair to topmost hash table

Retrieval:
- int symbol_table_scope_level(struct symbol_table* st)
	Returns the current number of in-scope tables in stack
- struct symbol* symbol_table_scope_lookup_at(struct symbol_table* st, const char* name, int index)
	Searches for a symbol by name given stack index
- struct symbol* symbol_table_scope_lookup(struct symbol_table* st, const char* name)
	Searches for a symbol by name considering all in-scope tables
- struct symbol* symbol_table_scope_lookup_all(struct symbol_table* st, const char* name)
	Searches for a symbol by name considering all tables in stack, regardless of scope
- struct symbol* symbol_table_scope_lookup_current(struct symbol_table* st, const char* name)
	Searches for a symbol by name considering only the topmost (current) scope

Printing:
- void symbol_table_fprint(FILE* fp, struct symbol_table* st)
	Prints a symbol table based on given file pointer
- void symbol_table_print(struct symbol_table* st)
	Prints symbol table to stdout

******************************************************************************************************************
TESTS
There are two test suites (in directory tests/symbol_table/):
The executables can be run individually via [tests/test_stack] or [tests/test_typecheck]
To run ALL tests run [test] (must be made via [make], [make all], or [make tests])

test_stack.c
  tests the symbol table structure
	- tests out 'generic' stack interface implementation
	- tests out hash table with symbol intgeration
	- tests out symbol table implementaiton (which automates all of the above)
	- tests out name resolution (which automates the symbol table implementation)

test_typecheck:
	tests out typechecking and error messages
	**Does not provide full coverage for the following:
		- statement error messages
		- symbol table error messages (and recovery)
		- return statement type checking
		- parameter list typechecking/redefinition

******************************************************************************************************************
CAVEATS:

 - 'delete' functions are called 'destroy' functions but they do the same thing.
 - symbol table API functions have a 'symbol_table_' prefix added to them to have consistent naming scheme with
   other functions in all the other files.
 - symbol_table_scope_bind() has return type of int
 - param_list_typecheck is NOT IMPLEMENTED.
     for determining if parameter lists are the same, param_list_equals() is called
     for determining if argument lists are the proper type, fcall_compare() is called
 - the following functions were changed from a void return type to an int return type:
      symbol_table:
        * symbol_table_scope_bind
      typechecking:
        * decl_typecheck
        * stmt_typecheck
    their return types were changed to int so that their error status able to be returned.
    this error status is the error code that is used by their corresponding error message handler.
