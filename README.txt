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

****************************
1. Register field of expr has default unused value of -1.
2. Which field of symbol has default unused value of -1.
3. scratch allocation failure returns -1 if no halt.
4. scratch name failure returns NULL if no halt.
5. label create failure returns INT_MIN if no halt.
6. symbol codegen failure returns NULL if no halt.



******************************************************************************************************
TO DO:

DONE1. implement + test necessary things for helper functions (expr->reg, symbol->which)
DONE2. implement + test helper functions (scratch, label)
DONE3. implement + test symbol_codegen


use assembly emulator to help you.
4. implement + test expr_codegen (the easy ones)
	- primitive expressions
		DONEimplement
		    test
	- arithmetic + logical expressions (excluding exponentiation)
		implement
		test
	- comparision operations
		implement
		test
	- other operations
		implement
		test
	- fcall
		implement
		test

5. Arrays and strings are passed by reference, meaning their labels have to be generated even if a literal.
   Therefore it is necessary to run symbol_codegen() throughout the symbol table prior to any <struct>_codegen() call.
   Though this would require adding literals to the symbol table, they can be 'hidden'. Stored in the table but never printed, their
   names are the label name generated from them via label_name(). This also requires #include for stmt.h and decl.h to have access to those
   functions.



CAVEATS:
*************************************************************************************************

- symbol table now includes 'hidden' symbols: originally nameless array and string literals stored as symbols under a new name: the label
  name generated from label_name() and label_create().This makes sure that pass-by-reference values are being stored and loaded correctly.
  These symbols are not shown as actual symbol table members, but can be with these functions:
	- symbol_table_print()
	- symbol_table_fprint()
	- hash_table_print()
	- hash_table_fprint()

  though this will require renaming these functions and let them take an additional boolean parameter for show_hidden and making wrapper
  functions:
	- symbol_table_print() and symbol_table_print_hidden() will then become wrapper functions for a function that uses the original
	  symbol_table_print() definition and an additional boolean parameter.
	- ditto with hash_table_print() and their fprint variants


- <<struct>>_codegen functions now have an 'fprintf' variant, as <<struct>>_fcodegen():
  These have an additonal parameter of the file pointer, which defaults to stdout.
