#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <stdbool.h>
#include <stdint.h>

// important enums
enum { CALLER = 0, CALLEE = 1, FARG = 3, NSCRATCH = 7, NFARG = 6 };
enum { BYTE = 1, WORD = 2, LONG = 4, QUAD = 8 };
typedef enum { REG_INVALID = 1, REG_AINUSE, REG_NINUSE, LABEL_MAX } reg_error_t;

// register structure
struct reg {
  const char* name;
  bool inuse;
  int kind; // caller, caller, farg
  int64_t value; // intermediate value tracking
}; typedef struct reg Register;

// register arrays
Register scratch_register[NSCRATCH];
Register farg_register[NFARG];


// other globals
int label_count;
int register_error_count;
int register_error_status;
bool is_test;

// functions

/*
Handles register/code generation errors
*/
void register_error_handle(reg_error_t kind, int ctx);

/*
Sets up the registers and label counter for code generation.
*/
void register_codegen_init(bool test);

/*
Clears label count and resets registers for code generation.
*/
void register_codegen_clear(void);

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
