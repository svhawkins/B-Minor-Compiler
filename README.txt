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
