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
   LABEL_MAX -> max number of labels

5. struct symbol now has an address field to have during code generation
   this saves the address value for a symbol while they are being generated.

6. symbol table now includes 'hidden' symbols: originally nameless array and string literals stored as symbols under a new name: the label
  name generated from label_name() and label_create().This makes sure that pass-by-reference values are being stored and loaded correctly.

7. <<struct>>_codegen functions now have an 'fprintf' variant, as <<struct>>_fcodegen():
  These have an additonal parameter of the file pointer, which defaults to stdout.

8. Updated typechecker for constant expressions.

9. Error handlers now display the error enum name to be more descriptive. [resolves isue #6]

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
	- comparision operations
		implement
		test
	- other operations
		implement
		test
	- fcall
		implement
		test


****NOTES

flags to implement
-o flag added to indicate output file. default name 'output.s'. can also just output redirection.
-i flag added to indicate input file, if any. defaults to stdin. can also just do input redirection.
-I flag added to indicate 'interactive'. input is stdin and output is stdout. this can be overwrites - or -i options.
-v flag added for 'verbose'. by default off. adds slight explanatory comments to the generated code.

since this only generates a .s file (human-readable assembly), the generated code is not guaranteed to work
if put through an assembler+linker+loader.