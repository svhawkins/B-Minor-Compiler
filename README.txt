[09:53-10:05] - make test_print, test_factory execs, update makefile + test
[11:00-11:30]
	update readme
	added driver code for assignment
        renamed main.c

current problems:
1. ast structures do not have proper destroy() functions

*********************************************
main.c for parse was renamed to parse.c

there are 3 test executables for ast testing

test_factory tests factory functions
	i got lost in pointer hell.
test_print tests print functions
test_ast tests the ast from bison, which integrates the factory and print functions


the factory and print function tests had the structures manually created per test.

new exec: ast? pretty_print? print? -> PRINT
uses new main.c


added grammar rule for auto keyword in type declarations
*************
features:
driver function has -t option (eventually)

all _print functions have new function struct_fprint, that prints to a given file stream:

void expr_fprint(FILE* fp, struct expr* e);
void type_fprint(FILE* fp, struct type* t);
void param_list_fprint(FILE* fp, struct param_list* p);
void decl_fprint(FILE* fp, struct decl* d, int indent);
void stmt_fprint(FILE* fp, struc stmt* s, int ident);

the original _print function from the starter code is just a wrapper to this function, using stdout as the stream:

ex) expr_print:
void expr_print(struct expr* e) { expr_fprint(stdout, e); }

This feature was added in order to test the print functions prior to bison integration, so that the
output could be saved somewhere.
(i tried this with a pipe, but it wouldn't work).

*** 
any problems/difficulties:

1. nothing so far. some might arise with the bison ast integration.

2. currently expr_print does not have the 'prettiest' printing, but it prints accurately according to associativity.
   there are too many parentheses.
   this is not a huge problem (just may hinder readability really), but not a big priority to have a PRETTY PRETTY PRINTER right now.


3. struct type now has additional field to hold its size when in declarations:
	struct type {
		int kind;
		struct type* subtype;
		struct param_list* params;
		struct expr* size;
	};
	this is used in type_print so that array types are properly printed with their size.
	also in cases of nested arrays:
	array [3] array [30 - 2] integer, the associated sizes remain with their subtypes.

	this also means type_create gets an updated signature.
