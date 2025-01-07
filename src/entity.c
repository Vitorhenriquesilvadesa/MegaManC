#include <entity.h>
#include <allocator.h>
#include <renderer.h>

Entity *newEntity(uint32_t type, EntityUpdateFn onUpdate, EntityCollisionFn onCollision, vec2s position, vec2s scale, vec2s aabbMin, vec2s aabbMax, bool isSolid, bool isVisible, SpriteRenderer *renderer)
{
    Entity *entity = ALLOCATE(Entity, 1);

    initEntity(entity, type, onUpdate, onCollision, position, scale, aabbMin, aabbMax, isSolid, isVisible, renderer);

    return entity;
}

void initEntity(Entity *entity, uint32_t type, EntityUpdateFn onUpdate, EntityCollisionFn onCollision, vec2s position, vec2s scale, vec2s aabbMin, vec2s aabbMax, bool isSolid, bool isVisible, SpriteRenderer *renderer)
{
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
    entity->isVisible = isVisible;
    entity->enableCollisions = true;

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
    updateAnimation(&entity->renderer->currentAnimation, dt);
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

void setAnimation(Entity *entity, Animation animation, AnimationPlay play, bool captureFrameTime)
{
    if (animation.texture == entity->renderer->currentAnimation.texture)
    {
        return;
    }

    uint32_t frameCount = animation.frameCount;
    uint32_t currentFrame = entity->renderer->currentAnimation.currentFrame;

    if (captureFrameTime)
    {
        animation.elapsedTime = 0.0f;
    }

    if (currentFrame > frameCount || play == PLAY_FROM_BEGIN)
    {
        currentFrame = 0;
    }

    entity->renderer->currentAnimation = animation;
    entity->renderer->currentAnimation.currentFrame = currentFrame;
}

bool isOnRightWall(Entity *entity)
{
    vec2s min = entity->collider.bound.min;
    vec2s max = entity->collider.bound.max;

    vec2s aabbPos = (vec2s){
        entity->transform.position.x + (min.x + max.x) / 2.0f,
        entity->transform.position.y + (min.y + max.y) / 2.0f};

    float halfHeight = (max.y - min.y) / 2.0f - 1.0f;
    float halfWidth = (max.x - min.x) / 2.0f + 1.0f;

    vec2s rayOriginT = {
        aabbPos.x + halfWidth + 1.0f,
        aabbPos.y + halfHeight};

    vec2s rayOriginM = {
        aabbPos.x + halfWidth + 1.0f,
        aabbPos.y};

    vec2s rayOriginB = {
        aabbPos.x + halfWidth + 1.0f,
        aabbPos.y - halfHeight};

    Ray raycastT = {rayOriginT, (vec2s){1.0f, 0.0f}};
    Ray raycastM = {rayOriginM, (vec2s){1.0f, 0.0f}};
    Ray raycastB = {rayOriginB, (vec2s){1.0f, 0.0f}};

    if (raycastHit(raycastM, 12))
    {
        return true;
    }
    else if (raycastHit(raycastB, 12))
    {
        return true;
    }

    return raycastHit(raycastT, 12);
}

bool isOnLeftWall(Entity *entity)
{
    vec2s min = entity->collider.bound.min;
    vec2s max = entity->collider.bound.max;

    vec2s aabbPos = (vec2s){
        entity->transform.position.x + (min.x + max.x) / 2.0f,
        entity->transform.position.y + (min.y + max.y) / 2.0f};

    float halfHeight = (max.y - min.y) / 2.0f - 1.0f;
    float halfWidth = (max.x - min.x) / 2.0f + 1.0f;

    vec2s rayOriginT = {
        aabbPos.x - halfWidth - 1.0f,
        aabbPos.y + halfHeight};

    vec2s rayOriginM = {
        aabbPos.x - halfWidth - 1.0f,
        aabbPos.y};

    vec2s rayOriginB = {
        aabbPos.x - halfWidth - 1.0f,
        aabbPos.y - halfHeight};

    Ray raycastT = {rayOriginT, (vec2s){-1.0f, 0.0f}};
    Ray raycastM = {rayOriginM, (vec2s){-1.0f, 0.0f}};
    Ray raycastB = {rayOriginB, (vec2s){-1.0f, 0.0f}};

    if (raycastHit(raycastM, 12))
    {
        return true;
    }
    else if (raycastHit(raycastB, 12))
    {
        return true;
    }

    return raycastHit(raycastT, 12);
}

bool isOnFloor(Entity *entity)
{
    vec2s min = entity->collider.bound.min;
    vec2s max = entity->collider.bound.max;

    vec2s aabbPos = (vec2s){
        entity->transform.position.x + (min.x + max.x) / 2.0f,
        entity->transform.position.y + (min.y + max.y) / 2.0f};

    float halfHeight = (max.y - min.y) / 2.0f;
    float halfWidth = (max.x - min.x) / 2.0f;

    vec2s rayOriginL = {
        aabbPos.x - halfWidth,
        aabbPos.y + halfHeight + 0.1f};

    vec2s rayOriginR = {
        aabbPos.x + halfWidth,
        aabbPos.y + halfHeight + 0.1f};

    Ray raycastL = {rayOriginL, (vec2s){0.0f, -1.0f}};
    Ray raycastR = {rayOriginR, (vec2s){0.0f, -1.0f}};

    if (raycastHit(raycastR, 9))
    {
        return true;
    }

    return raycastHit(raycastL, 9);
}

bool isOnCeil(Entity *entity)
{
    vec2s min = entity->collider.bound.min;
    vec2s max = entity->collider.bound.max;

    vec2s aabbPos = (vec2s){
        entity->transform.position.x + (min.x + max.x) / 2.0f,
        entity->transform.position.y + (min.y + max.y) / 2.0f};

    float halfHeight = (max.y - min.y) / 2.0f;
    float halfWidth = (max.x - min.x) / 2.0f;

    vec2s rayOriginL = {
        aabbPos.x - halfWidth,
        aabbPos.y - halfHeight - 0.1f};

    vec2s rayOriginR = {
        aabbPos.x + halfWidth,
        aabbPos.y - halfHeight - 0.1f};

    Ray raycastL = {rayOriginL, (vec2s){0.0f, 1.0f}};
    Ray raycastR = {rayOriginR, (vec2s){0.0f, 1.0f}};

    if (raycastHit(raycastR, 10))
    {
        return true;
    }

    return raycastHit(raycastL, 10);
}