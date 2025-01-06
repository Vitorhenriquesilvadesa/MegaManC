#include <aabb.h>
#include <allocator.h>
#include <entity.h>
#include <game.h>
#include <graphics_api.h>

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

    Transform transformA = a->transform;
    Transform transformB = b->transform;

    if (coliderA.bound.max.x + transformA.position.x < coliderB.bound.min.x + transformB.position.x ||
        coliderA.bound.min.x + transformA.position.x > coliderB.bound.max.x + transformB.position.x)
    {
        return false;
    }

    if (coliderA.bound.max.y + transformA.position.y < coliderB.bound.min.y + transformB.position.y ||
        coliderA.bound.min.y + transformA.position.y > coliderB.bound.max.y + transformB.position.y)
    {
        return false;
    }

    return true;
}

AABBColisionData calculateCollisionData(Entity *a, Entity *b)
{
    AABB colA = a->collider;
    AABB colB = b->collider;

    Transform transformA = a->transform;
    Transform transformB = b->transform;

    vec2s aMin = glms_vec2_add(colA.bound.min, transformA.position);
    vec2s aMax = glms_vec2_add(colA.bound.max, transformA.position);
    vec2s bMin = glms_vec2_add(colB.bound.min, transformB.position);
    vec2s bMax = glms_vec2_add(colB.bound.max, transformB.position);

    float overlapX1 = bMax.x - aMin.x;
    float overlapX2 = aMax.x - bMin.x;
    float overlapY1 = bMax.y - aMin.y;
    float overlapY2 = aMax.y - bMin.y;

    AABBColisionData data;
    data.other = b;
    data.overlap = 0;
    data.normal = (vec2s){0.0f, 0.0f};

    if (overlapX1 > 0 && overlapX2 > 0 && overlapY1 > 0 && overlapY2 > 0)
    {
        float overlapX = fminf(overlapX1, overlapX2);
        float overlapY = fminf(overlapY1, overlapY2);

        if (overlapX < overlapY)
        {
            data.overlap |= (overlapX1 < overlapX2) ? OVERLAP_LEFT : OVERLAP_RIGHT;
            data.normal = (vec2s){(overlapX1 < overlapX2) ? -1.0f : 1.0f, 0.0f};
        }
        else
        {
            data.overlap |= (overlapY1 > overlapY2) ? OVERLAP_DOWN : OVERLAP_UP;
            data.normal = (vec2s){0.0f, (overlapY1 < overlapY2) ? -1.0f : 1.0f};
        }
    }

    return data;
}

void resolveCollision(Entity *a, Entity *b, AABBColisionData *collisionData)
{
    vec2s move = {0.0f, 0.0f};

    Entity *movable = (!a->isSolid && b->isSolid) ? a : (!b->isSolid && a->isSolid) ? b
                                                                                    : NULL;
    if (!movable)
        return;

    AABB colA = a->collider;
    AABB colB = b->collider;

    Transform transformA = a->transform;
    Transform transformB = b->transform;

    vec2s aMin = glms_vec2_add(colA.bound.min, transformA.position);
    vec2s aMax = glms_vec2_add(colA.bound.max, transformA.position);
    vec2s bMin = glms_vec2_add(colB.bound.min, transformB.position);
    vec2s bMax = glms_vec2_add(colB.bound.max, transformB.position);

    float overlapX1 = bMax.x - aMin.x;
    float overlapX2 = aMax.x - bMin.x;
    float overlapY1 = bMax.y - aMin.y;
    float overlapY2 = aMax.y - bMin.y;

    float overlapX = fminf(overlapX1, overlapX2);
    float overlapY = fminf(overlapY1, overlapY2);

    if (overlapX < overlapY)
    {
        move.x = (overlapX1 < overlapX2 ? -overlapX : overlapX);
    }
    else
    {
        move.y = (overlapY1 < overlapY2 ? -overlapY : overlapY);
    }

    move = glms_vec2_add(move, glms_vec2_scale(collisionData->normal, 0.01f));

    movable->transform.position = glms_vec2_add(movable->transform.position, move);
}

Entity *newCollider(vec2s position, vec2s size)
{
    static SpriteRenderer *renderer;

    if (!renderer)
    {
        GraphicsAPI *graphics = (GraphicsAPI *)getGameInstanceService(SERVICE_TYPE_GRAPHICS);

        Shader *shader = getShader(graphics, SHADER_TYPE_SPRITE);
        Texture *texture = newTextureFromImage("../assets/sprites/null/null.png");
        Animation animation = newAnimation(01, 01, texture, false, PLAY_FROM_BEGIN);
        renderer = newSpriteRenderer(shader, animation);
    }

    Entity *entity = newEntity(ENTITY_TYPE_COLLIDER, onUpdateNull, onCollisionNull,
                               position, size,
                               (vec2s){0.0f, 0.0f},
                               (vec2s){size.x, size.y},
                               true, false, renderer);

    return entity;
}

void onUpdateNull(void *self, float dt)
{
}

void onCollisionNull(void *self, AABBColisionData data)
{
}
