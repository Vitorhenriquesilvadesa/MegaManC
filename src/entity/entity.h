#ifndef ENTITY_H
#define ENTITY_H

#define CGLM_USE_STRUCT
#include <cglm/struct.h>

#include <sprite.h>

typedef struct
{
    vec2s position;
    vec2s scale;
    SpriteRenderer *renderer;
} Entity;

Entity *newEntity(vec2s position, vec2s scale, SpriteRenderer *renderer);
mat4s entityGetTransformationMatrix(Entity *entity);

void updateEntity(Entity *entity, float dt);

#endif