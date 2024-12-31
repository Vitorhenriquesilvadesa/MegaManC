#include <memory.h>
#include <stdlib.h>

void *allocate(size_t count)
{
    void *ptr = malloc(count);
    return ptr;
}

void *reallocate(void *ptr, size_t new_size)
{
    void *new_ptr = realloc(ptr, new_size);
    return new_ptr;
}
