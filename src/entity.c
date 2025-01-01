#include <entity.h>
#include <allocator.h>

Entity *newEntity(vec2s position, vec2s scale, SpriteRenderer *renderer)
{
    Entity *entity = ALLOCATE(Entity, 1);
    entity->position = position;
    entity->scale = scale;
    entity->renderer = renderer;

    return entity;
}

mat4s entityGetTransformationMatrix(Entity *entity)
{
    vec3s position = {entity->position.x, entity->position.y, 0.0f};
    vec3s scale = {entity->scale.x, entity->scale.y, 1.0};

    mat4s transformation = GLMS_MAT4_IDENTITY_INIT;
    transformation = glms_translate(transformation, position);
    transformation = glms_scale(transformation, scale);

    return transformation;
}

void updateEntity(Entity *entity, float dt)
{
    updateAnimation(entity->renderer->currentAnimation, dt);
}
