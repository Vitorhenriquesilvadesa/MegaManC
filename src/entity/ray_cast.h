#ifndef RAY_CAST_H
#define RAY_CAST_H

#define CGLM_USE_STRUCT
#include <cglm/struct.h>

#include <stdbool.h>
#include <aabb.h>

typedef struct
{
    vec2s origin;
    vec2s direction;
} Ray;

bool rayIntersectsAABB(vec2s rayOrigin, vec2s rayDir, Entity *entity, float distance);
bool raycastHit(Ray ray, float distance);

#endif