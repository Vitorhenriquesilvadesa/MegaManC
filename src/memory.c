#include <memory.h>
#include <stdlib.h>
#include "allocator.h"

size_t bytesAllocated = 0;

void *allocate(size_t count)
{
    bytesAllocated += count;
    void *ptr = malloc(count);
    return ptr;
}

void *reallocate(void *ptr, size_t new_size)
{
    void *new_ptr = realloc(ptr, new_size);
    return new_ptr;
}

void freePtr(void *ptr)
{
    free(ptr);
}

size_t getBytesAllocated()
{
    return bytesAllocated;
}
