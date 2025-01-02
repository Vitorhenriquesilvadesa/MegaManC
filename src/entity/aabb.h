#ifndef AABB_H
#define AABB_H

#define CGLM_USE_STRUCT
#include <cglm/struct.h>

typedef struct Entity Entity;

typedef struct
{
    struct bound
    {
        vec2s min;
        vec2s max;
    } bound;
} AABB;

typedef struct
{
    AABB a;
    AABB b;
    vec2s normal;
} AABBColisionData;

AABB *newAABB(vec2s min, vec2s max);
bool AABBIntersect(struct Entity *a, struct Entity *b);

#endif