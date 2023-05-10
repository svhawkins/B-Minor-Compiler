#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

// important enums
enum { CALLER = 0, CALLEE = 1, NSCRATCH = 7, NFARG = 6 };
enum { BYTE = 1, WORD = 2, LONG = 4, QUAD = 8 };

// register structure
struct reg {
  const char* name;
  bool inuse;
  int kind; // caller, caller, farg
}; typedef struct reg Register;

// register arrays
Register scratch_register[NSCRATCH];
Register farg_register[NFARG];


// other globals
int label_count;


// functions

/*
Marks a register as inuse.
Fails if no registers are available to use.
Halt in cases of failure.
*/
int register_scratch_alloc();

/*
Unmarks an inuse register as not in use given by index r.
Failure if:
	- r is out of bounds (0 < r <= NSCRATCH)
	- no registers are in use
Halt in cases of failure.
*/
void register_scratch_free(int r);

/*
Returns name of the register given by index r.
Failure if:
	- r is out of bounds (0 < r <= NSCRATCH)
Halt in cases of failure.
*/
const char* register_scratch_name(int r);

/*
Creates a label number value
Failure if:
	- integer overflow
Halt in cases of failure.
*/
int label_create();

/*
Creates a label string value
*/
const char* label_name(int label);
