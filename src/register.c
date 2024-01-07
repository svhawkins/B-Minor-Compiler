#include "register.h"

char* scratch_name[NSCRATCH] = { "%rbx", "%r10", "%r11", "%r12", "%r13", "%r14", "%r15" };
char* scratch_name_32[NSCRATCH] = {"%ebx", "%r10d", "%r11d", "%r12d", "%r13d", "%r14d", "%r15d" };
char* scratch_name_16[NSCRATCH] = {"%bx", "%r10w", "%r11w", "%r13w", "%r14w", "%r15w"};
char* scratch_name_high[NSCRATCH] = {"%bh", NULL, NULL, NULL, NULL, NULL, NULL};
char* scratch_name_low[NSCRATCH] = {"%bl", "%r10b", "%r11b", "%r12b", "%r13b", "%r14b", "%r15b"};
int scratch_kind[NSCRATCH] = { CALLEE, CALLER, CALLER, CALLEE, CALLEE, CALLEE, CALLEE};
char buffer[16];

/*
Handles register/code generation errors
*/
char* register_strerror(reg_error_t kind) {
  switch(kind) {
    case REG_INVALID: strcpy(buffer, "EREGINVALID"); break;
    case REG_AINUSE: strcpy(buffer, "EREGAINUSE"); break;
    case REG_NINUSE: strcpy(buffer, "EREGNINUSE"); break;
    case REG_NOHIGH: strcpy(buffer, "EREGNOHIGH"); break;
    case LABEL_MAX: strcpy(buffer, "EREGMAX"); break;
  }
  return buffer;
}
void register_error_handle(reg_error_t kind, int ctx) {
  fprintf(REG_ERR_OUT, "CODEGEN ERROR: %s (%d)\n\t", register_strerror(kind), kind);
  switch(kind) {
    case REG_INVALID: /* out of bounds register selected */
      fprintf(REG_ERR_OUT, "register index %d is out of bounds.\n", ctx);
      fprintf(REG_ERR_OUT, "\tscratch register indices are between 0 and 6 (inclusive).\n");
      fprintf(REG_ERR_OUT, "\tfarg register indices are between 0 and 5 (inclusive).\n");
      break;
    case REG_AINUSE: /* all registers are in use */
      fprintf(REG_ERR_OUT, "all registers are currently in use.\n\tfailed to allocate for another.\n");
      break;
    case REG_NINUSE: /* selected register is already free to use */
      fprintf(REG_ERR_OUT, "selected register %s is already not in use.\n", scratch_register[ctx].name);
      break;
    case REG_NOHIGH: /* selected register does not have a high byte form */
      fprintf(REG_ERR_OUT, "64-bit scratch register %s (index %d) does not have requested 8-bit high byte register.\n",
                            scratch_register[ctx].name, ctx);
      fprintf(REG_ERR_OUT, "\tthe only 64-bit scratch register with 8-bit high byte register is %rbx (index 0): %bl.\n");
      break;
    case LABEL_MAX: /* max number of labels has been reached */
      fprintf(REG_ERR_OUT, "failed to create new label. Maximum has been reached.\n");
      break;
  }
  fprintf(REG_ERR_OUT, "\n");
  register_error_status = kind;
  register_error_count++;
  if (!is_test) { exit(kind); }
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
  if (index < 0) { register_error_handle(REG_AINUSE, -1); }
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
  else if (!scratch_register[r].inuse) { register_error_handle(REG_NINUSE, r); }
  else { scratch_register[r].inuse = false; }
}

/*
Returns name of the 64-bit register given by index r.
Failure if:
        - r is out of bounds (0 < r <= NSCRATCH)
Halt in cases of failure.
*/
const char* register_scratch_name(int r) {
  if (r < 0 || r >= NSCRATCH) { register_error_handle(REG_INVALID, -1); return NULL; }
  return scratch_register[r].name;
}

/*
Returns name of the 32-bit register given by index r.
Failure if:
	- r is out of bounds (0 < r <= NSCRATCH)
Halt in cases of failure.
*/
const char* register_scratch_name_32(int r) {
  if (r < 0 || r >= NSCRATCH) { register_error_handle(REG_INVALID, -1); return NULL; }
  return scratch_name_32[r];
}


/*
Returns name of the 16-bit register given by index r.
Failure if:
	- r is out of bounds (0 < r <= NSCRATCH)
Halt in cases of failure.
*/
const char* register_scratch_name_16(int r) {
  if (r < 0 || r >= NSCRATCH) { register_error_handle(REG_INVALID, -1); return NULL; }
  return scratch_name_16[r];
}


/*
Returns name of the 8-bit high byte register given by index r.
Failure if:
	- r is out of bounds (0 < r <= NSCRATCH)
  - r does not have a corresponding high-byte form
Halt in cases of failure.
*/
const char* register_scratch_name_high(int r) {
  if (r < 0 || r >= NSCRATCH) { register_error_handle(REG_INVALID, -1); return NULL; }
  else if (r != 0) { register_error_handle(REG_NOHIGH, r); return NULL; }
  return scratch_name_high[r];
}


/*
Returns name of the 8-bit low byte register given by index r.
Failure if:
	- r is out of bounds (0 < r <= NSCRATCH)
Halt in cases of failure.
*/
const char* register_scratch_name_low(int r) {
  if (r < 0 || r >= NSCRATCH) { register_error_handle(REG_INVALID, -1); return NULL; }
  return scratch_name_low[r];
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
