#include <entity.h>
#include <allocator.h>

Entity *newEntity(EntityType type, EntityUpdateFn update, vec2s position, vec2s scale, SpriteRenderer *renderer)
{
    Entity *entity = ALLOCATE(Entity, 1);

    entity->type = type;
    entity->update = update;
    entity->position = position;
    entity->scale = scale;
    entity->renderer = renderer;
    entity->isMirrored = false;

    return entity;
}

void initEntity(Entity *entity, EntityType type, EntityUpdateFn update, vec2s position, vec2s scale, SpriteRenderer *renderer)
{
    entity->type = type;
    entity->update = update;
    entity->position = position;
    entity->scale = scale;
    entity->renderer = renderer;
    entity->isMirrored = false;
}

mat4s entityGetTransformationMatrix(Entity *entity)
{
    vec3s position = {entity->position.x, entity->position.y, 0.0f};
    vec3s scale = {entity->scale.x, entity->scale.y, 1.0};

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
    entity->update(entity, dt);
    updateAnimation(entity->renderer->currentAnimation, dt);
}

void freeEntity(Entity *entity)
{
    freeSpriteRenderer(entity->renderer);
    FREE(entity);
}
