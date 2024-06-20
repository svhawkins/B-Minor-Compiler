#ifndef VECTOR_H
#define VECTOR_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


struct vector {
  void** items;
  int size;
  int capacity;
}; typedef struct vector Vector;

struct int_vector {
  int* items;
  int size;
  int capacity;
}; typedef struct int_vector intVector;


/*
creates and allocates memory for a vector
returns NULL upon failure.
*/
Vector* vector_create();
intVector* intVector_create();

/*
frees the memory allocated by the vector and its items.
sets vector pointer to NULL upon success.
*/
void vector_destroy(Vector** v);
void intVector_destroy(intVector** v);


/*
Adds item to the vector, increasing its size
Memory may have to be reallocated if above capacity (left shift)
*/
void vector_push(Vector* v, void* item);
void intVector_push(intVector* v, int item);


/*
Pops the topmost item from the vector, removing it.
This item is returned.
The vector size decreases.
Memory may have to be reallocated if (way) under capacity (right shift)
*/
void* vector_pop(Vector* v);
int intVector_pop(intVector* v);


/*
returns vector size (number of items) within the vector
*/
int vector_size(Vector* v);
int intVector_size(intVector* v);


/*
Returns to pointer to item specified by position.
The top of the vector is the final item in the vector (size - 1)
*/
void* vector_item(Vector* v, int position);
int intVector_item(intVector* v, int position);

#endif /* VECTOR_H */