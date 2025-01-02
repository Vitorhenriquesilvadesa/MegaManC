#include <entity.h>
#include <allocator.h>
#include <renderer.h>

Entity *newEntity(EntityType type, EntityUpdateFn onUpdate, EntityCollisionFn onCollision, vec2s position, vec2s scale, vec2s aabbMin, vec2s aabbMax, bool isSolid, SpriteRenderer *renderer)
{
    Entity *entity = ALLOCATE(Entity, 1);

    initEntity(entity, type, onUpdate, onCollision, position, scale, aabbMin, aabbMax, isSolid, renderer);

    return entity;
}

void initEntity(Entity *entity, EntityType type, EntityUpdateFn onUpdate, EntityCollisionFn onCollision, vec2s position, vec2s scale, vec2s aabbMin, vec2s aabbMax, bool isSolid, SpriteRenderer *renderer)
{
    float minX = -scale.x / 2.0f;
    float maxX = scale.x / 2.0f;
    float minY = -scale.y / 2.0f;
    float maxY = scale.y / 2.0f;

    AABB collider;

    vec2s aabbSize = (vec2s){aabbMax.x - aabbMin.x, aabbMax.y - aabbMin.y};

    collider.bound.min = (vec2s){-aabbSize.x / 2.0f, -aabbSize.y / 2.0f};
    collider.bound.max = (vec2s){aabbSize.x / 2.0f, aabbSize.y / 2.0f};

    entity->type = type;
    entity->onUpdate = onUpdate;
    entity->onCollision = onCollision;
    entity->transform.position = position;
    entity->transform.scale = scale;
    entity->renderer = renderer;
    entity->isMirrored = false;
    entity->isSolid = isSolid;

    entity->collider = collider;
}

mat4s entityGetTransformationMatrix(Entity *entity)
{
    vec3s position = {entity->transform.position.x, entity->transform.position.y, 0};
    vec3s scale = {entity->transform.scale.x, entity->transform.scale.y, 1.0};

    mat4s transformation = GLMS_MAT4_IDENTITY_INIT;
    transformation = glms_translate(transformation, position);

    if (entity->isMirrored)
    {
        transformation = glms_scale(transformation, (vec3s){-scale.x, scale.y, scale.z});
    }
    else
    {
        transformation = glms_scale(transformation, scale);
    }

    return transformation;
}

void updateEntity(Entity *entity, float dt)
{
    entity->onUpdate(entity, dt);
    updateAnimation(entity->renderer->currentAnimation, dt);
}

void freeEntity(Entity *entity)
{
    freeSpriteRenderer(entity->renderer);
    FREE(entity);
}

bool isEntityOnScreen(Entity *entity, Camera2D *camera)
{
    vec2s pos = entity->transform.position;
    vec2s camPos = camera->position;
    vec2s scale = entity->transform.scale;

    return (
        pos.x > camPos.x - 128 - scale.x / 2.0f &&
        pos.x < camPos.x + 128 + scale.x / 2.0f &&
        pos.y > camPos.y - 112 - scale.y / 2.0f &&
        pos.y < camPos.y + 128 + scale.y / 2.0f);
}
