COMP 4060 201
Assignment 5: Code Generator
*******************************************************************************************************

Changes since assignment 4:
1. [main.c] renamed to [typecheck.c]
2. symbol_table_clear() now no longer always creates non-verbose symbol tables.
   If the just destroyed table was verbose, then so is the new one created.
3. 2 tests were added in tests/symbol_table/test_data_structures.c to test for correct generation of symbol->which values
   upon scope entry and exit through binding via <struct>_resolve()
4. Added 2 additional code files:
   register.h/register.c to handle all of the register allocation (and labels) for code generation
   the scratch register functions and label functions are found here.
   - also contains init/clear functions for easier setup/desetup.
   - also contains error message handler with its own error counter.
   - register_codegen_init() also toggles the global is_test through its bool parameter. when true,
     this makes the error message handler not halt/exit the program and instead simply return.

   REG_INVALID -> out of bounds
   REG_AINUSE -> all registers in use
   REG_NINUSE -> register not in use
   REG_NOHIGH -> register name is unavailable in the high-byte bit type (64, 32, 16, high, low)
   LABEL_MAX -> max number of labels

5. struct symbol now has an address field to have during code generation
   this saves the address value for a symbol while they are being generated.

6. symbol table now includes 'hidden' symbols: originally nameless array and string literals stored as symbols under a new name: the label
  name generated from label_name() and label_create(). This makes sure that pass-by-reference values are being stored and loaded correctly.

7. Updated typechecker for constant expressions.

8. Error handlers now display the error enum name to be more descriptive. [resolves isue #6]

9. Various code cleanups for better readability, code-style consistency, and better cohesion.

****************************
FILES
****************************
EXECUTABLES
****************************
ERROR MESSAGES
****************************
TESTS
****************************
HIDDEN SYMBOLS

symbol_table.h/.c now has an additional structure to hold these hidden symbols: Hidden_table.
Under the hood still very much a hash table (much like how the Symbol_table's vector is).

Additional functions have been added for Hidden_table:


For typechecking:
- Hidden_table* symbol_table_hidden_create()
- void symbol_table_hidden_destroy(Hidden_table** hst)
- int symbol_table_hidden_bind(Hidden_table* hst, const char* literal)
- int symbol_table_hidden_lookup(Hidden_table* hst, const char* literal)

For symbol table printing:
- void symbol_table_hidden_fprint(FILE* fp, Hidden_table* hst)
- void symbol_table_hidden_print(Hidden_table* hst)

For code generation:
- void symbol_table_hidden_codegen(Hidden_table* hst)

The hidden table is an additional field of the Symbol_table structure. 
The hidden symbols are generated seperately from the rest of the declarations (attempts were made to make it part
of the same declaration list, but did not work, so it has its own special function).

*****************
 CAVEATS
*****************


TO DO:

DONE1. implement + test necessary things for helper functions (expr->reg, symbol->which)
DONE2. implement + test helper functions (scratch, label)
DONE3. implement + test symbol_codegen


use assembly emulator to help you.
4. implement + test expr_codegen (the easy ones)
	DONE- primitive expressions
		DONEimplement
		DONEtest
	DONE- arithmetic + logical expressions (excluding exponentiation/division/multiplication)
		DONEimplement
		DONEtest
	DONE- relational expressions
		DONEimplement
		DONEtest
	- other operations
		implement
		test
	- fcall (not necessarily declaration, just prologues and epilogues)
		implement
		test

5. implement + test decl codegen
   DONE- global non-function/array declarations
   DONE- local non-function/array declarations
   - array declarations (global, local) (requires EXPR_INIT)

6. implement + test stmt codegen
   - expression statements
   - declaration statements
   - code blocks
   - print statements
   - return statements
   - loops
   - conditional statements

7. functions:
   - function declarations


****NOTES

the code generator only generates a .s file. it still needs to be assembled, linked, and loaded afterward.

these flags can be part of the compiler proper (bcc --> B-Minor C Compiler (blind carbon copy (bcc) of gcc...))
-S: do NOT assemble the generated (.s) code (ie don't let it go through as)
-c: do NOT link the assembled (.o) code (ie don't let it go through ld)
-o: output file
-v: verbose (show argv for every executed program)

runtime library functions:
- exponentiation (pow)
- printing (printf)
- string comparision (==, !=) (strcmp)

*written in C, compile BEFORE linking with B-Minor programs. can be compiled with the rest of the compiler as .a.

similar behavior with storing literals from names:
decl codegen associates expression value with name (value determined via its own expr codegen),
which can then be further referenced in expr codegen for intermediate value tracking.
now intermediate value tracking of values works for both literals and names.


constant expressions in global scope must NOT have their full expression generated, but rather the resulting value.
entirely constant expressions (ie not containing only literals) can be optimised to NOT generate the expression(s) to
reach their value, just that a register be stored with the end result.