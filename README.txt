

******************************************
current problems/bugs:
1. ast structures do not have proper destroy() functions

2. name for expr_create_name is not being stored properly.
   giving funny things like
   x + y; + y; for x + y;

   x[i][j];[i][j];]; for x[i][j]
   with all the other literals it works PERFECTLY fine (very good actually), so I am not sure where it is going wrong.
   I really don't understand. With MANUALLY created ast structures, this isn't a problem. So it's not the print
   function, meaning it is bison. 
   
   Update: I managed to fix it a little bit, just had to 'clean' the value.

3. subscript lists do not nest for arbitrary dimension
   I was having problems figuring out the proper rules/format for the proper ast construction, so I
   just applied a really bad band-aid fix where the first 5 dimensions are hard-coded. I can fix it later,
   but it works for now (WHO is going to need a 5-dimensional array).

   Luckily, hard-coding the dimensions follows a pattern:
	1. The left branch of the next expression is the entirety of the previous expression.
	2. The base case is $2, where its left subtree is NOT a subscript rule, but the name.
	3. The resulting tree ($$) is the final subscript rule.

	If I have time later I can make a better rule for this, but not right now.

4. Function call lists are not yet supported.
   Cannot do things like f(x)(y), but composition is possible: f(x(y))
   Similar problems to what is described in (3).
   (because of this f(x)[i] doesn't quite yet work)

*********************************************
main.c for parse was renamed to parse.c
make [test_all] renamed to make [tests]

there are 3 test executables for ast testing

test_factory tests factory functions
	i got lost in pointer hell.
test_print tests print functions
test_ast tests the ast from bison, which integrates the factory and print functions


the factory and print function tests had the structures manually created per test in test_print and test_factory

new exec: print
uses new main.c
	print is more or less the same program as parse, except that it stops reading (valid) input once there is EOF/^D.
        it then prints out what was inputted, prettily!

	if there was invalid input, then no input is printed. just error messages like before.


added grammar rule for auto keyword in type declarations
made exponent grammar rule be right associative instead of left associative

unary negation and binary subtraction are two seperate operations for ast purposes
unary plus and binary addition are two seperate operations for ast purposes

added additional expression kinds:
expr_comma for expression lists
expr_init for initialization lists

*************
features:
driver function has -t option just like in [parse], since this main program is just a continuation of the validator

all <struct>_print functions have new function <struct>_fprint, that prints to a given file stream:

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



*** 
any problems/difficulties:

1. struct type now has additional field to hold its size when in declarations:
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

2. difficulty: integrating the semantic types with the parser
   why it was difficult: I did NOT #include all the necessary files and LINK them with any executable/.c file
   that used the parser/semantic types after implementing and testing them.
   (that is, linker was yelling at me because test_parse executable had undefined references to factory
    functions because it was now included in the grammar, which was not the case when the parser and its test
    code was only a validator)

   several hours were wasted figuring it out :/.

3. while statements are implemented
