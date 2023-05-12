#include "register.h"

char label_str[8]; // max int 5 digits, 3 for .L and \0
char* scratch_name[NSCRATCH] = { "%%rbx", "%%r10", "%%r11", "%%r12", "%%r13", "%%r14", "%%r15" };
int scratch_kind[NSCRATCH] = { CALLEE, CALLER, CALLER, CALLEE, CALLEE, CALLEE, CALLEE};

/*
Handles register/code generation errors
*/
void register_error_handle(reg_error_t kind, int ctx) {
  fprintf(stderr, "CODEGEN ERROR: %d", kind);
  switch(kind) {
    case REG_INVALID: /* out of bounds register selected */
      fprintf(stderr, "register index %d is out of bounds.\n", ctx);
      fprintf(stderr, "scratch register indices are between 0 and 6 (inclusive).");
      fprintf(stderr, "farg register indices are between 0 and 5 (inclusive).");
      break;
    case REG_AINUSE: /* all registers are in use */
      fprintf(stderr, "all registers are currently in use.\nfailed to allocated for another.");
      break;
    case REG_NINUSE: /* selected register is already free to use */
      fprintf(stderr, "selected register %s is already not in use.\n", scratch_register[ctx].name);
      break;
    case LABEL_MAX: /* max number of labels has been reached */
      fprintf(stderr, "failed to create new label. maximum has been reached.\n");
      break;
  }
  fprintf(stderr, "\n");
  register_error_status = kind;
  register_error_count++;
  if (!is_test) exit(kind);
}

/*
Sets up the registers and label counter for code generation.
*/
void register_codegen_init(bool test) {
  is_test = test;
  register_error_count = 0; register_error_status = 0;
  label_count = -1;
  for (int i = 0; i < NSCRATCH; i++) {
    scratch_register[i].name = scratch_name[i];
    scratch_register[i].inuse = false;
    scratch_register[i].kind = scratch_kind[i];
  }
}

/*
Clears label count and resets registers for code generation.
*/
void register_codegen_clear(void) {
  label_count = -1;
  for (int i = 0; i < NSCRATCH; i++) { scratch_register[i].inuse = false; }
  register_error_count = 0;
  register_error_status = 0;
}


/*
Marks a register as inuse, returning the index of the now inuse register.
Failure if:
	- no available registers
Halt in cases of failure, (returns -1)
*/
int register_scratch_alloc() {
  int index = -1;
  for (int i = 0; i < NSCRATCH; i++) {
    if (!scratch_register[i].inuse) { scratch_register[i].inuse = true; index = i; break; }
  }
  if (index < 0) register_error_handle(REG_AINUSE, -1);
  return index;
}

/*
Unmarks an inuse register as not in use given by index r.
Failure if:
        - r is out of bounds (0 < r <= NSCRATCH)
        - no registers are in use
Halt in cases of failure.
*/
void register_scratch_free(int r) {
  if (r < 0 || r >= NSCRATCH) { register_error_handle(REG_INVALID, r); }
  if (!scratch_register[r].inuse) { register_error_handle(REG_NINUSE, r); }
  else { scratch_register[r].inuse = false; }
}

/*
Returns name of the register given by index r.
Failure if:
        - r is out of bounds (0 < r <= NSCRATCH)
Halt in cases of failure.
*/
const char* register_scratch_name(int r) {
  if (r < 0 || r >= NSCRATCH) { register_error_handle(REG_INVALID, -1); return NULL; }
  return scratch_register[r].name;
}

/*
Creates a label number value
Failure if:
        - integer overflow
Halt in cases of failure.
*/
int label_create() {
  label_count++;
  if (label_count < 0) { register_error_handle(LABEL_MAX, -1); }
  return label_count;
}

/*
Creates a label string value
*/
const char* label_name(int label) {
  sprintf(label_str, ".L%d", label);
  return label_str;
}
