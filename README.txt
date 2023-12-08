COMP 4060 201
Assignment 5: Code Generator
*******************************************************************************************************

Changes since assignment 4:
1. [main.c] renamed to [typecheck.c]
2. symbol_table_clear() now no longer always creates non-verbose symbol tables.
   if the just destroyed table was verbose, then so is the new one created.
3. 2 tests were added in tests/symbol_table/test_stack.c to test for correct generation of symbol->which values
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

9. Various code cleanups for better readability and code-style consistency.

****************************
HIDDEN SYMBOLS


******************************************************************************************************
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
	- fcall
		implement
		test


****NOTES

flags to implement

in setting the codegen in/out flags for file pointers.
-o flag added to indicate output file. default name 'output.s'. can also just output redirection.
-i flag added to indicate input file, if any. defaults to stdin. can also just do input redirection.
-I flag added to indicate 'interactive'. input is stdin and output is stdout. this can be overwrites -o or -i options.


-v flag added for 'verbose'. by default off. adds slight explanatory comments to the generated code.

since this only generates a .s file (human-readable assembly), the generated code is not guaranteed to work
if put through an assembler+linker+loader.

these flags can be part of the compiler proper (bcc --> B-Minor C Compiler (blind carbon copy (bcc) of gcc...))
-S: do NOT assemble the generated (.s) code (ie don't let it go through as)
-c: do NOT link the assembled (.o) code (ie don't let it go through ld)
-o: output file
-v: verbose (show argv for every executed program)




exponentation and printing are put in the runtime library which can either be statically or dynamically
linked. static linking may be easier. 
this runtime library is to be written in C, it has to be compiled to object code before linking with B-Minor programs.

use a hidden symbol table (additional standalone hashtable) instead of adding hidden symbols to symbol table.
this hash table associates string literals to label names (whereas in the real symbol table it's the other way around).
expr codegen associates a string literal to a label.
   upon finding a string literal, it finds associated 'hidden' label and assigns it to a string literal expession's name field.
   decl codegen will use this name instead of the string literal value.

   this hash table is to be printed seperately from the declaration list.

similar behavior with storing literals from names:
decl codegen associates expression value with name (value determined via its own expr codegen),
which can then be further referenced in expr codegen for intermediate value tracking.
now intermediate value tracking of values works for both literals and names.


constant expressions in global scope must NOT have their full expression generated, but rather the resulting value.
entirely constant expressions (ie not containing only literals) can be optimised to NOT generate the expression(s) to
reach their value, just that a register be stored with the end result.