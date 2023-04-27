#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


struct stack {
  void** items;
  int size;
  int capacity;
}; typedef struct stack Stack;




/*
creates and allocates memory for a stack
returns NULL upon failure.
*/
Stack* stack_create();

/*
frees the memory allocated by the stack and its items.
sets stack pointer to NULL upon success.
*/
void stack_destroy(Stack** s);


/*
Adds item to the stack, increasing its size
Memory may have to be reallocated if above capacity (left shift)
*/
void stack_push(Stack* s, void* item);


/*
Pops the topmost item from the stack, removing it.
This item is returned.
The stack size decreases.
Memory may have to be reallocated if (way) under capacity (right shift)
*/
void* stack_pop(Stack* s);


/*
returns stack size (number of items) within the stack
*/
int stack_size(Stack* s);


/*
Returns to pointer to item specified by position.
The top of the stack is the final item in the vector (size - 1)
*/
void* stack_item(Stack* s, int position);
