#ifndef ENTITY_H
#define ENTITY_H

#define CGLM_USE_STRUCT
#include <cglm/struct.h>

#include <sprite.h>

#define CAST_ENTITY(type, value) type *entity = (type *)value

#define AS_ENTITY_PTR(value) (Entity *)value

typedef void (*EntityUpdateFn)(void *self, float dt);

typedef enum
{
    ENTITY_TYPE_MEGAMAN,
    ENTITY_TYPE_BRICK,
} EntityType;

typedef struct
{
    EntityType type;
    EntityUpdateFn update;
    vec2s position;
    vec2s scale;
    bool isMirrored;
    SpriteRenderer *renderer;
} Entity;

Entity *newEntity(EntityType type, EntityUpdateFn update, vec2s position, vec2s scale, SpriteRenderer *renderer);
void initEntity(Entity *entity, EntityType type, EntityUpdateFn update, vec2s position, vec2s scale, SpriteRenderer *renderer);

mat4s entityGetTransformationMatrix(Entity *entity);

void updateEntity(Entity *entity, float dt);
void freeEntity(Entity *entity);

#endif