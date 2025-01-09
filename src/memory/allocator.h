#ifndef MEMORY_H
#define MEMORY_H

#include <stdint.h>
#include <stdlib.h>

#define ARRAY_MIN_SIZE 8

void *allocate(size_t count);
void *reallocate(void *ptr, size_t new_size);
void freePtr(void *ptr);

size_t getBytesAllocated();

#define REALLOCATE(type, ptr, new_count) \
    (type *)reallocate(ptr, (new_count) * sizeof(type))

#define ALLOCATE(type, count) (type *)allocate(count * sizeof(type))

#define FREE(ptr) freePtr(ptr)

#define ARRAY_GROW_FACTOR 2

#endif