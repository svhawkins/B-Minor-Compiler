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

****************************
1. Register field of expr has default unused value of -1.
2. Which field of symbol has default unused value of -1.



******************************************************************************************************
TO DO:

DONE1. implement + test necessary things for helper functions (expr->reg, symbol->which)
2. implement + test helper functions (scratch, label)
3. implement + test symbol_codegen


use assembly emulator to help you.
4. implement + test expr_codegen (the easy ones)
