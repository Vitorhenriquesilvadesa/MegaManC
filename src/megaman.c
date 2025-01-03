#include <megaman/megaman.h>
#include <sizes.h>
#include <game.h>
#include <object_pool_api.h>
#include <graphics_api.h>
#include <allocator.h>
#include <ray_cast.h>
#include <GLFW/glfw3.h>

#define MEGAMAN_MAX_SPEED 90.0f
#define MEGAMAN_AIR_MAX_FALL_SPEED 400.0f
#define MEGAMAN_AIR_JUMP_STRENGTH 345.0f
#define MEGAMAN_GRAVITY 900.0f

static MegamanResources megamanResources = {NULL, NULL, NULL};

static void createAnimations()
{
    if (!megamanResources.walk)
    {
        Texture *texture = newTextureFromImage("../assets/sprites/megaman/walk.png");
        megamanResources.walk = newAnimation(4, 10, texture, true, PLAY_FROM_BEGIN);
    }

    if (!megamanResources.jump)
    {
        Texture *texture = newTextureFromImage("../assets/sprites/megaman/jump.png");
        megamanResources.jump = newAnimation(1, 1, texture, false, PLAY_FROM_BEGIN);
    }

    if (!megamanResources.idle)
    {
        Texture *texture = newTextureFromImage("../assets/sprites/megaman/idle.png");
        megamanResources.idle = newAnimation(15, 10, texture, true, PLAY_FROM_BEGIN);
    }
}

Megaman *newMegaman(vec2s position)
{
    GraphicsAPI *graphics = (GraphicsAPI *)getGameInstanceService(SERVICE_TYPE_GRAPHICS);

    Shader *shader = getShader(graphics, SHADER_TYPE_SPRITE);

    createAnimations();

    SpriteRenderer *renderer = newSpriteRenderer(shader, megamanResources.idle);
    Entity entity;

    initEntity(&entity, ENTITY_TYPE_MEGAMAN, onUpdateMegaman, onCollisionMegaman, position, MEGAMAN_SIZE,
               (vec2s){10.0f, 3.0f}, (vec2s){20.0f, 26.0f}, false, renderer);

    Megaman *megaman = ALLOCATE(Megaman, 1);
    megaman->entity = entity;
    megaman->maxFallSpeed = MEGAMAN_AIR_MAX_FALL_SPEED;
    megaman->gravity = MEGAMAN_GRAVITY;
    megaman->maxSpeed = MEGAMAN_MAX_SPEED;
    megaman->jumpStrength = MEGAMAN_AIR_JUMP_STRENGTH;
    megaman->speed = (vec2s){0.0f, 0.0f};

    return megaman;
}

static bool isOnRightWall(Entity *entity);
static bool isOnLeftWall(Entity *entity);
static bool isOnFloor(Entity *entity);
static bool isOnCeil(Entity *entity);

void onUpdateMegaman(void *self, float dt)
{
    Megaman *megaman = (Megaman *)self;

    GraphicsAPI *graphics = (GraphicsAPI *)getGameInstanceService(SERVICE_TYPE_GRAPHICS);

    megaman->isRightWall = isOnRightWall((Entity *)self);
    megaman->isLeftWall = isOnLeftWall((Entity *)self);
    megaman->isOnFloor = isOnFloor((Entity *)self);
    megaman->isOnCeil = isOnCeil((Entity *)self);

    megaman->speed.x = -isKeyPressed(GLFW_KEY_LEFT) * megaman->maxSpeed + isKeyPressed(GLFW_KEY_RIGHT) * megaman->maxSpeed;

    megaman->isMoving = megaman->speed.x != 0.0f;

    if (isKeyPressed(GLFW_KEY_LEFT))
    {
        megaman->entity.isMirrored = true;
    }

    if (isKeyPressed(GLFW_KEY_RIGHT))
    {
        megaman->entity.isMirrored = false;
    }

    if (!isKeyPressed(GLFW_KEY_Z) && megaman->speed.y > 100.0f)
    {
        megaman->speed.y = 100.0f;
    }

    if (megaman->isMoving && megaman->isOnFloor)
    {
        setAnimation(&megaman->entity, megamanResources.walk, PLAY_FROM_BEGIN);
    }

    if (!megaman->isMoving && megaman->isOnFloor)
    {
        setAnimation(&megaman->entity, megamanResources.idle, PLAY_FROM_BEGIN);
    }

    if (!megaman->isOnFloor)
    {
        setAnimation(&megaman->entity, megamanResources.jump, PLAY_FROM_BEGIN);
    }

    if (!megaman->isOnFloor)
    {
        megaman->speed.y -= megaman->gravity * dt;
    }

    static bool floorSet = false;
    static bool ceilSet = false;

    if (megaman->isOnFloor && !floorSet)
    {
        floorSet = true;
        megaman->speed.y = 0.0f;
    }

    if (megaman->isOnCeil && !ceilSet)
    {
        ceilSet = true;
        megaman->speed.y = 0.0f;
    }

    if (!megaman->isOnFloor)
    {
        floorSet = false;
    }

    if (!megaman->isOnCeil)
    {
        ceilSet = false;
    }

    if (!isKeyPressed(GLFW_KEY_LEFT) && !isKeyPressed(GLFW_KEY_RIGHT))
    {
        megaman->speed.x = 0.0f;
    }

    static bool jumped = false;

    if (isKeyPressed(GLFW_KEY_Z) && megaman->isOnFloor && !jumped)
    {
        jumped = true;
        megaman->speed.y = megaman->jumpStrength;
    }

    if (!isKeyPressed(GLFW_KEY_Z))
    {
        jumped = false;
    }

    if (megaman->speed.x > 0.0f && !megaman->isRightWall)
    {
        megaman->entity.transform.position.x += megaman->speed.x * dt;
    }

    if (megaman->speed.x < 0.0f && !megaman->isLeftWall)
    {
        megaman->entity.transform.position.x += megaman->speed.x * dt;
    }

    megaman->entity.transform.position.y -= megaman->speed.y * dt;

    graphics->renderer->camera->position.x = megaman->entity.transform.position.x;
}

static bool isOnRightWall(Entity *entity)
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

static bool isOnLeftWall(Entity *entity)
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

static bool isOnFloor(Entity *entity)
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

static bool isOnCeil(Entity *entity)
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

void onCollisionMegaman(void *self, AABBColisionData data)
{
}
