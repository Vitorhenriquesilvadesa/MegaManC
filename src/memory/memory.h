#ifndef MEMORY_H
#define MEMORY_H

#include <stdint.h>

void *allocate(size_t count);
void *reallocate(void *ptr, size_t new_size);

#define REALLOCATE(type, ptr, old_count, new_count) \
    (type *)reallocate(ptr, new_count * sizeof(type))

#define ALLOCATE(type, count) (type *)allocate(count * sizeof(type))

#define FREE(ptr) free(ptr)

#define ARRAY_GROW_FACTOR 2

#endif