#ifndef ENTITY_H
#define ENTITY_H

#define CGLM_USE_STRUCT
#include <cglm/struct.h>

#include <sprite.h>
#include <aabb.h>

#define CAST_ENTITY(type, value) type *entity = (type *)value

#define AS_ENTITY_PTR(value) (Entity *)value

typedef struct Camera2D Camera2D;

typedef void (*EntityUpdateFn)(void *self, float dt);
typedef void (*EntityCollisionFn)(void *self, AABBColisionData data);

typedef enum
{
    ENTITY_TYPE_MEGAMAN,
    ENTITY_TYPE_BRICK,
} EntityType;

typedef struct
{
    vec2s position;
    vec2s scale;
} Transform;

typedef struct Entity
{
    EntityType type;
    EntityUpdateFn onUpdate;
    EntityCollisionFn onCollision;
    Transform transform;
    bool isMirrored;
    bool isSolid;
    SpriteRenderer *renderer;
    AABB collider;
} Entity;

Entity *newEntity(EntityType type, EntityUpdateFn onUpdate, EntityCollisionFn onCollision, vec2s position, vec2s scale, vec2s aabbMin, vec2s aabbMax, bool isSolid, SpriteRenderer *renderer);
void initEntity(Entity *entity, EntityType type, EntityUpdateFn onUpdate, EntityCollisionFn onCollision, vec2s position, vec2s scale, vec2s aabbMin, vec2s aabbMax, bool isSolid, SpriteRenderer *renderer);
mat4s entityGetTransformationMatrix(Entity *entity);

void updateEntity(Entity *entity, float dt);
void freeEntity(Entity *entity);
bool isEntityOnScreen(Entity *entity, struct Camera2D *camera);
void setAnimation(Entity *entity, Animation *animation, AnimationPlay play);

#endif