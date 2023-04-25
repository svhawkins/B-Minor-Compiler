#include "stack.h"

/*
creates and allocates memory for a stack
returns NULL upon failure:
  - stack allocation failure
  - items allocation failure
*/
Stack* stack_create() {
  Stack* s = (Stack*)malloc(sizeof(s));
  if (!s) { /* display error message....*/ }
  if (s) {
   s->size = 0;
   s->capacity = 1<<3;
   s->items = (void**)calloc(s->capacity, sizeof(void*));
   if (!s->items) { free(s); /* display error message...*/ }
  }
  return s;
}

/*
frees the memory allocated by the stack and its items.
sets stack pointer to NULL once done
*/
void stack_destroy(Stack** s) {
  if (*s) {
    // clear all elements?
    free((*s)->items);
    free(*s); *s = NULL;
  }
}


/*
Adds item to the stack, increasing its size
Memory may have to be reallocated if above capacity (left shift)

if in any case of failure:
  - unallocated stack
  - unallocated items
  - memory reallocation
item is set to NULL.
*/
void stack_push(Stack* s, void* item) {
  if (!s || !s->items) return;
  if (s->size >= s->capacity) {
    int new_capacity = s->capacity << 1;
    void** new_buffer = (void**)calloc(new_capacity, sizeof(void*));
    if (new_buffer) {
      for (int i = 0; i < s->size; i++) new_buffer[i] = s->items[i];
      free(s->items); s->items = NULL;
      s->items = new_buffer;
      s->capacity = new_capacity;
    }
  }
  s->items[s->size] = item;
  s->size++;
}


/*
Pops the topmost item from the stack, removing it.
This item is returned.
The stack size decreases.
Memory may have to be reallocated if (way) under capacity (right shift)

If in any case of failure:
  - unallocated stack
  - unallocated items
  - empty stack
  - memory reallocation
return NULL
*/
void* stack_pop(Stack* s) {
  if (!(s && s->items && s->size)) return NULL;
  int new_capacity = s->capacity >> 1;
  if ((s->size - 1) < new_capacity) {
   void** new_buffer = (void**)calloc(new_capacity, sizeof(void*));
   if (new_buffer) {
      for (int i = 0; i < s->size; i++) new_buffer[i] = s->items[i];
      free(s->items); s->items = NULL;
      s->items = new_buffer;
      s->capacity = new_capacity;
    }
  }
  int top = s->size - 1;
  void* item = s->items[top]; // whatever the topmost value is
  s->items[top] = NULL;
  s->size--;
  return item;
}


/*
returns stack size (number of items) within the stack
Returns -1 if no stack nor items vector has been allocated.
*/
int stack_size(Stack* s) { return (s && s->items) ? s->size : -1; }


/*
Returns to pointer to item specified by position.
The top of the stack is final element in items (size - 1)
If either:
   - out of bounds (negative or above size)
   - stack or items is unallocated
return NULL.
*/
void* stack_item(Stack* s, int position) { return ((s && s->items) && (position >= 0 && position < s->size)) ? s->items[position] : NULL; }
