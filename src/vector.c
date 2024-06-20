#include "vector.h"

/*
creates and allocates memory for a vector
returns NULL upon failure:
  - vector allocation failure
  - items allocation failure
*/
Vector* vector_create() {
  Vector* v = (Vector*)malloc(sizeof(Vector));
  if (!v) { fprintf(stderr, "WARNING: Failed to allocate memory space for vector!\n"); }
  if (v) {
   v->size = 0;
   v->capacity = 1<<3;
   v->items = (void**)calloc(v->capacity, sizeof(void*));
   if (!v->items) { free(v); fprintf(stderr, "WARNING: Failed to allocate memory space for vector contents!\n"); }
  }
  return v;
}

intVector* intVector_create() {
  intVector* v = (intVector*)malloc(sizeof(intVector));
  if (!v) { fprintf(stderr, "WARNING: Failed to allocate memory space for vector!\n"); }
  if (v) {
   v->size = 0;
   v->capacity = 1<<3;
   v->items = (int*)calloc(v->capacity, sizeof(int));
   if (!v->items) { free(v); fprintf(stderr, "WARNING: Failed to allocate memory space for vector contents!\n"); }
  }
  return v;
}

/*
frees the memory allocated by the vector and its items.
sets vector pointer to NULL once done
*/
void vector_destroy(Vector** v) {
  if (v && *v) {
    free((*v)->items);
    free(*v);
    *v = NULL;
  }
}

void intVector_destroy(intVector** v) { vector_destroy(v); }


/*
Adds item to the vector, increasing its size
Memory may have to be reallocated if above capacity (left shift)
if in any case of failure:
  - unallocated vector
  - unallocated items
  - memory reallocation
item is set to NULL.
*/
void vector_push(Vector* v, void* item) {
  if (!v || !v->items) { return; }
  if (v->size >= v->capacity) {
    int new_capacity = v->capacity << 1;
    void** new_buffer = (void**)calloc(new_capacity, sizeof(void*));
    if (new_buffer) {
      for (int i = 0; i < v->size; i++) { new_buffer[i] = v->items[i]; }
      free(v->items); v->items = NULL;
      v->items = new_buffer;
      v->capacity = new_capacity;
    }
  }
  v->items[v->size] = item;
  v->size++;
}

void intVector_push(intVector* v, int item) {
  if (!v || !v->items) { return; }
  if (v->size >= v->capacity) {
    int new_capacity = v->capacity << 1;
    int* new_buffer = (int*)calloc(new_capacity, sizeof(int));
    if (new_buffer) {
      for (int i = 0; i < v->size; i++) { new_buffer[i] = v->items[i]; }
      free(v->items); v->items = NULL;
      v->items = new_buffer;
      v->capacity = new_capacity;
    }
  }
  v->items[v->size] = item;
  v->size++;
}


/*
Pops the topmost item from the vector, removing it.
This item is returned.
The vector size decreases.
Memory may have to be reallocated if (way) under capacity (right shift)

If in any case of failure:
  - unallocated vector
  - unallocated items
  - empty vector
  - memory reallocation
return NULL
*/
void* vector_pop(Vector* v) {
  if (!(v && v->items && v->size)) { return NULL; }
  int new_capacity = v->capacity >> 1;
  if ((v->size - 1) < new_capacity) {
   void** new_buffer = (void**)calloc(new_capacity, sizeof(void*));
   if (new_buffer) {
      for (int i = 0; i < v->size; i++) new_buffer[i] = v->items[i];
      free(v->items); v->items = NULL;
      v->items = new_buffer;
      v->capacity = new_capacity;
    }
  }
  int top = v->size - 1;
  void* item = v->items[top];
  v->items[top] = NULL;
  v->size--;
  return item;
}

int intVector_pop(intVector* v) {
  if (!(v && v->items && v->size)) { return NULL; }
  int new_capacity = v->capacity >> 1;
  if ((v->size - 1) < new_capacity) {
   int* new_buffer = (int*)calloc(new_capacity, sizeof(int));
   if (new_buffer) {
      for (int i = 0; i < v->size; i++) new_buffer[i] = v->items[i];
      free(v->items); v->items = NULL;
      v->items = new_buffer;
      v->capacity = new_capacity;
    }
  }
  int top = v->size - 1;
  void* item = v->items[top];
  v->items[top] = NULL;
  v->size--;
  return item;
}


/*
returns vector size (number of items) within the vector
Returns -1 if no vector nor items vector has been allocated.
*/
int vector_size(Vector* v) { return (v && v->items) ? v->size : -1; }
int intVector_size(intVector* v) { return vector_size(v); }


/*
Returns to pointer to item specified by position.
The top of the vector is final element in items (size - 1)
If either:
   - out of bounds (negative or above size)
   - vector or items is unallocated
return NULL.
*/
void* vector_item(Vector* v, int position) {
  return ((v && v->items) && (position >= 0 && position < v->size)) ? v->items[position] : NULL;
}

int intVector_item(intVector* v, int position) {
  return ((v && v->items) && (position >= 0 && position < v->size)) ? v->items[position] : 0;
}
