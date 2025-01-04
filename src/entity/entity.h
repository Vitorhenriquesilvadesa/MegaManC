#ifndef ENTITY_H
#define ENTITY_H

#define CGLM_USE_STRUCT
#include <cglm/struct.h>

#include <sprite.h>
#include <aabb.h>
#include <types.h>

#define CAST_ENTITY(type, value) type *entity = (type *)value

#define AS_ENTITY_PTR(value) ((Entity *)value)

typedef struct Camera2D Camera2D;

typedef void (*EntityUpdateFn)(void *self, float dt);
typedef void (*EntityCollisionFn)(void *self, AABBColisionData data);

typedef enum
{
    ENTITY_TYPE_MEGAMAN = BIT(0),
    ENTITY_TYPE_BRICK = BIT(1),
    ENTITY_TYPE_COLLIDER = BIT(2),
} EntityType;

typedef struct
{
    vec2s position;
    vec2s scale;
} Transform;

typedef struct Entity
{
    uint32_t type;
    EntityUpdateFn onUpdate;
    EntityCollisionFn onCollision;
    Transform transform;
    bool isMirrored;
    bool isSolid;
    bool isVisible;
    SpriteRenderer *renderer;
    AABB collider;
} Entity;

Entity *newEntity(uint32_t type, EntityUpdateFn onUpdate, EntityCollisionFn onCollision, vec2s position, vec2s scale, vec2s aabbMin, vec2s aabbMax, bool isSolid, bool isVisible, SpriteRenderer *renderer);
void initEntity(Entity *entity, uint32_t type, EntityUpdateFn onUpdate, EntityCollisionFn onCollision, vec2s position, vec2s scale, vec2s aabbMin, vec2s aabbMax, bool isSolid, bool isVisible, SpriteRenderer *renderer);
mat4s entityGetTransformationMatrix(Entity *entity);

void updateEntity(Entity *entity, float dt);
void freeEntity(Entity *entity);
bool isEntityOnScreen(Entity *entity, struct Camera2D *camera);
void setAnimation(Entity *entity, Animation *animation, AnimationPlay play, bool captureFrameTime);

#endif