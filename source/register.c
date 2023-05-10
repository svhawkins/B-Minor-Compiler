#include "register.h"

/*
TO DO:
- set up the register tables/arrays
- implement the functions
- test the functions
*/

/*
Marks a register as inuse.
Fails if no registers are available to use.
Halt in cases of failure.
*/
int register_scratch_alloc() { return 0; }

/*
Unmarks an inuse register as not in use given by index r.
Failure if:
        - r is out of bounds (0 < r <= NSCRATCH)
        - no registers are in use
Halt in cases of failure.
*/
void register_scratch_free(int r) {}

/*
Returns name of the register given by index r.
Failure if:
        - r is out of bounds (0 < r <= NSCRATCH)
Halt in cases of failure.
*/
const char* register_scratch_name(int r) { return NULL; }

/*
Creates a label number value
Failure if:
        - integer overflow
Halt in cases of failure.
*/
int label_create() { return 0; }

/*
Creates a label string value
*/
const char* label_name(int label) { return NULL; }
