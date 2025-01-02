#include <aabb.h>
#include <allocator.h>
#include <entity.h>

AABB *newAABB(vec2s min, vec2s max)
{
    AABB *collider = ALLOCATE(AABB, 1);

    collider->bound.min = min;
    collider->bound.max = max;

    return collider;
}

bool AABBIntersect(Entity *a, Entity *b)
{
    AABB coliderA = a->collider;
    AABB coliderB = b->collider;

    if (coliderA.bound.max.x + a->position.x < coliderB.bound.min.x + b->position.x ||
        coliderA.bound.min.x + a->position.x > coliderB.bound.max.x + b->position.x)
    {
        return false;
    }

    if (coliderA.bound.max.y + a->position.y < coliderB.bound.min.y + b->position.y ||
        coliderA.bound.min.y + a->position.y > coliderB.bound.max.y + b->position.y)
    {
        return false;
    }

    return true;
}