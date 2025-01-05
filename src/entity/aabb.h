#ifndef AABB_H
#define AABB_H

#define CGLM_USE_STRUCT
#include <cglm/struct.h>
#include <stdint.h>

typedef struct Entity Entity;

typedef enum
{
    OVERLAP_LEFT = (1 << 0),
    OVERLAP_RIGHT = (1 << 1),
    OVERLAP_UP = (1 << 2),
    OVERLAP_DOWN = (1 << 3),
} AABBOverlap;

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
    Entity *other;
    vec2s normal;
    uint8_t overlap;
} AABBColisionData;

AABB *newAABB(vec2s min, vec2s max);
bool AABBIntersect(struct Entity *a, struct Entity *b);
AABBColisionData calculateCollisionData(Entity *a, Entity *b);
void resolveCollision(Entity *a, Entity *b, AABBColisionData *collisionData);
Entity *newCollider(vec2s position, vec2s size);
void onCollisionNull(void *self, AABBColisionData data);
void onUpdateNull(void *self, float dt);

#endif