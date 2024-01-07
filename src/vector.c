#include "vector.h"

/*
creates and allocates memory for a vector
returns NULL upon failure:
  - vector allocation failure
  - items allocation failure
*/
Vector* vector_create() {
  Vector* s = (Vector*)malloc(sizeof(Vector));
  if (!s) { fprintf(stderr, "WARNING: Failed to allocate memory space for vector!\n"); }
  if (s) {
   s->size = 0;
   s->capacity = 1<<3;
   s->items = (void**)calloc(s->capacity, sizeof(void*));
   if (!s->items) { free(s); fprintf(stderr, "WARNING: Failed to allocate memory space for vector contents!\n"); }
  }
  return s;
}

/*
frees the memory allocated by the vector and its items.
sets vector pointer to NULL once done
*/
void vector_destroy(Vector** s) {
  if (*s) {
    free((*s)->items);
    free(*s); *s = NULL;
  }
}


/*
Adds item to the vector, increasing its size
Memory may have to be reallocated if above capacity (left shift)
if in any case of failure:
  - unallocated vector
  - unallocated items
  - memory reallocation
item is set to NULL.
*/
void vector_push(Vector* s, void* item) {
  if (!s || !s->items) { return; }
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
void* vector_pop(Vector* s) {
  if (!(s && s->items && s->size)) { return NULL; }
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
  void* item = s->items[top];
  s->items[top] = NULL;
  s->size--;
  return item;
}


/*
returns vector size (number of items) within the vector
Returns -1 if no vector nor items vector has been allocated.
*/
int vector_size(Vector* s) { return (s && s->items) ? s->size : -1; }


/*
Returns to pointer to item specified by position.
The top of the vector is final element in items (size - 1)
If either:
   - out of bounds (negative or above size)
   - vector or items is unallocated
return NULL.
*/
void* vector_item(Vector* s, int position) {
  return ((s && s->items) && (position >= 0 && position < s->size)) ? s->items[position] : NULL;
}
