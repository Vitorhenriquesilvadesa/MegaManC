#ifndef ITERATOR_H
#define ITERATOR_H

#define FOR_EACH(type, collection, code)               \
    do                                                 \
    {                                                  \
        size_t _i;                                     \
        for (_i = 0; _i < (collection).length; ++_i)   \
        {                                              \
            type *element = (collection).services[_i]; \
            code;                                      \
        }                                              \
    } while (0)

#endif