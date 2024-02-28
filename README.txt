COMP 4060 201
Assignment 5: Code Generator
*******************************************************************************************************

Changes since assignment 4:

1. Name changes + Benign file changes:
   - [main.c] renamed to [typecheck.c]
   - [stack.c], [stack.h] renamed to [vector.c] and [vector.h]
   - [test_data_structures.c] renamed to [test_data_structures.c] as it tests out more than just the former stack (now vector):
         - vector functionality
         - vector functionality with hash table integration
         - symbol table functionality
   - [test_stack] (now [test_data_structures]) split into 2 files:
         - [test_data_structures.c] to explicity test data structure functionality
         - [test_resolve.c] (former test_symbol_table.c) to test automatic symbol table integration with <struct>_resolve().
         
2. Symbol table:
   - symbol_table_clear() now no longer always creates non-verbose symbol tables.
      If the destroyed table was verbose, then so is the new one created.
   - Minor internal structure changes due to a bug in scope entry/exits [POSTPONED]
   - Now includes 'hidden' symbols: originally nameless array and string literals stored as symbols
     under a new name: the label name generated from label_name() and label_create().
     This makes sure that pass-by-reference string values are being stored and loaded correctly.

3. Structures
   - struct symbol now has an address field to have during code generation
      this saves the address value for a symbol while they are being generated.
   - Updated typechecker for constant expresion checking
   - Error handlers now display the error enum name to be more descriptive.

4. Misc:
   - Various code cleanups for better readability, code-style consistency, and better cohesion.
   - executables now have help command line options (-h, --help)

****************************
FILES:
Other than the addition of the code generation functions for the structures, new files have been added for register 
handling: register.h and register.c. They handle register and label allocation. Also contains helper functions
for initialization and clearing for easier setup/cleanup. Also has its own set of error messages with its own
handler (which in occurence of an error simply exits if not explicitly stated for a test).

Tree:
****************************
EXECUTABLES

// codegen executables
// features/commands/how to build
****************************
ERROR MESSAGES

Error messages can now be fatal (errors) or non-fatal (warnings). Errors stop code generation
and exit. Warnings do continue with generation, though results may be undefined or unwarranted.

Register Error Messages:
   REG_INVALID -> out of bounds
   REG_AINUSE -> all registers in use
   REG_NINUSE -> register not in use
   REG_NOHIGH -> register name is unavailable in the high-byte bit type (64, 32, 16, high, low)
   LABEL_MAX -> max number of labels

Codegen Error Messages:
  Expressions:
      Errors:
         EXPR_BYZERO --> Division or Modulus by zero deteted
      Warnings:
         ERR_OVERFLOW --> Integer overflow detected
         ERR_UNDERFLOW --> Integer underflow detected
  Statements:
      Errors:
      Warnings:
  Declarations:
      Errors:
         DECL_NEGSIZE --> Array's declared size evaluated to negative
      Warnings:
         DECL_SIZE --> Array's evaluated declared size is smaller than initializer list size
         DECL_PADSIZE --> Array's evaluated declared size is larger than intializer list size, list is zero-padded.
****************************
TESTS

The code generator has seperate code files for each generation component*:

[test_sym_codegen.c]
   * Tests Register and label handling, symbol code generation, and hidden symbol management
- [test_expr_codegen.c]
   * Tests expr_codegen
- [test_decl_codegen.c]
   * Tests decl_codegen for primitive function declarations
- [test_decl_array_codegen.c]
   * Tests decl_codegen for array declarations
- [test_stmt_codegen.c] --> TODO
   * Tests stmt_codegen 
- [test_codegen] --> TODO
   * Tests the code generator at a 'program' level, using test programs

*they were originally all together, but I kept running into memory issues, the issue went away once splitting them apart.
(stack overflow perhaps?)

****************************
HIDDEN SYMBOLS

symbol_table.h/.c now has an additional structure to hold these hidden symbols: Hidden_table.
Pretty much a typedef hash table.

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

 - some functions (namely the codegen functions) have had their signatures modified.
   Some return an error status and expr_codegen takes an additonal boolean option.

 - expr_codegen only does 'intermediate value tracking' (evaluates the expression during generation) with literals (so far).
   if an expression contains an identifier and has underflow, overflow, divsion-by-zero, etc.,
   the code generator will fail to express that.
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
	- fcall (not necessarily declaration, just stuff before and after call)
		implement
		test

5. implement + test decl codegen
   DONE- global non-function/array declarations
   DONE- local non-function/array declarations
   DONE- array declarations (global, local) (requires EXPR_INIT)
   - multidimensional array declarations

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
      - function prologue
      - function epilogue
      - function bodies
      - nested function calls


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