#include <megaman/megaman.h>
#include <sizes.h>
#include <game.h>
#include <object_pool_api.h>
#include <graphics_api.h>
#include <trigger.h>
#include <allocator.h>
#include <ray_cast.h>
#include <GLFW/glfw3.h>

#define MEGAMAN_MAX_SPEED 90.0f
#define MEGAMAN_AIR_MAX_FALL_SPEED 300.0f
#define MEGAMAN_AIR_JUMP_STRENGTH 345.0f
#define MEGAMAN_GRAVITY 1000.0f

static MegamanResources megamanResources = {};

static void createAnimations()
{
    megamanResources.normalAnimations = ALLOCATE(Animation, MEGAMAN_STATE_MAX);
    megamanResources.shootingAnimations = ALLOCATE(Animation, MEGAMAN_STATE_MAX);

    {
        Texture *texture = newTextureFromImage("../assets/sprites/megaman/walk.png");
        megamanResources.normalAnimations[MEGAMAN_STATE_WALK] = newAnimation(4, 10, texture, true, PLAY_FROM_BEGIN);
    }

    {
        Texture *texture = newTextureFromImage("../assets/sprites/megaman/jump.png");
        megamanResources.normalAnimations[MEGAMAN_STATE_JUMP] = newAnimation(1, 1, texture, false, PLAY_FROM_BEGIN);
    }

    {
        Texture *texture = newTextureFromImage("../assets/sprites/megaman/idle.png");
        megamanResources.normalAnimations[MEGAMAN_STATE_IDLE] = newAnimation(15, 10, texture, true, PLAY_FROM_BEGIN);
    }

    {
        Texture *texture = newTextureFromImage("../assets/sprites/megaman/walk_shoot.png");
        megamanResources.shootingAnimations[MEGAMAN_STATE_WALK] = newAnimation(4, 10, texture, true, PLAY_FROM_BEGIN);
    }

    {
        Texture *texture = newTextureFromImage("../assets/sprites/megaman/jump_shoot.png");
        megamanResources.shootingAnimations[MEGAMAN_STATE_JUMP] = newAnimation(1, 1, texture, false, PLAY_FROM_BEGIN);
    }

    {
        Texture *texture = newTextureFromImage("../assets/sprites/megaman/idle_shoot.png");
        megamanResources.shootingAnimations[MEGAMAN_STATE_IDLE] = newAnimation(1, 1, texture, true, PLAY_FROM_BEGIN);
    }
}

Megaman *newMegaman(vec2s position)
{
    GraphicsAPI *graphics = (GraphicsAPI *)getGameInstanceService(SERVICE_TYPE_GRAPHICS);

    Shader *shader = getShader(graphics, SHADER_TYPE_SPRITE);

    static bool animationsCreated = false;

    if (!animationsCreated)
    {
        createAnimations();
        animationsCreated = true;
    }

    SpriteRenderer *renderer = newSpriteRenderer(shader, megamanResources.normalAnimations[MEGAMAN_STATE_IDLE]);
    Entity entity;

    initEntity(&entity, ENTITY_TYPE_MEGAMAN, onUpdateMegaman, onCollisionMegaman, position, MEGAMAN_SIZE,
               (vec2s){8.0f, 3.0f}, (vec2s){22.0f, 24.0f}, false, true, renderer);

    Megaman *megaman = ALLOCATE(Megaman, 1);
    megaman->entity = entity;
    megaman->shootTime = 0.0f;
    megaman->maxFallSpeed = MEGAMAN_AIR_MAX_FALL_SPEED;
    megaman->gravity = MEGAMAN_GRAVITY;
    megaman->maxSpeed = MEGAMAN_MAX_SPEED;
    megaman->jumpStrength = MEGAMAN_AIR_JUMP_STRENGTH;
    megaman->state = MEGAMAN_STATE_IDLE;
    megaman->speed = (vec2s){0.0f, 0.0f};
    megaman->jumpTriggered = false;

    setAnimation(&megaman->entity, megamanResources.normalAnimations[megaman->state], PLAY_FROM_BEGIN, false);

    TriggerAPI *triggers = (TriggerAPI *)getGameInstanceService(SERVICE_TYPE_EVENT);
    registerTrigger(triggers, onMegamanShootTrigger, onMegamanShoot, megaman);
    registerTrigger(triggers, onMegamanCeilTrigger, onMegamanCeil, megaman);
    registerTrigger(triggers, onMegamanLandTrigger, onMegamanLand, megaman);
    registerTrigger(triggers, onMegamanJumpTrigger, onMegamanJump, megaman);

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

    static bool shoot = false;

    if (megaman->shootTime <= 0.0f)
    {
        setAnimation(&megaman->entity, megamanResources.normalAnimations[megaman->state], PLAY_FROM_BEGIN, false);
    }
    else
    {
        megaman->shootTime -= dt;
        setAnimation(&megaman->entity, megamanResources.shootingAnimations[megaman->state], PLAY_FROM_CURRENT_FRAME, true);
    }

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
        megaman->state = MEGAMAN_STATE_WALK;
    }

    if (!megaman->isMoving && megaman->isOnFloor)
    {
        megaman->state = MEGAMAN_STATE_IDLE;
    }

    if (!megaman->isOnFloor)
    {
        megaman->state = MEGAMAN_STATE_JUMP;
    }

    if (!megaman->isOnFloor && megaman->speed.y > -megaman->maxFallSpeed)
    {
        megaman->speed.y -= megaman->gravity * dt;
    }

    if (megaman->speed.y < -megaman->maxFallSpeed)
    {
        megaman->speed.y = -megaman->maxFallSpeed;
    }

    if (!isKeyPressed(GLFW_KEY_LEFT) && !isKeyPressed(GLFW_KEY_RIGHT))
    {
        megaman->speed.x = 0.0f;
    }

    if (megaman->speed.x > 0.0f && !megaman->isRightWall || megaman->speed.x < 0.0f && !megaman->isLeftWall)
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

bool onMegamanShootTrigger(void *self)
{
    Megaman *megaman = (Megaman *)self;
    return isKeyPressed(GLFW_KEY_X);
}

bool onMegamanJumpTrigger(void *self)
{
    Megaman *megaman = (Megaman *)self;

    bool zPressed = isKeyPressed(GLFW_KEY_Z);

    if (!zPressed && megaman->isOnFloor)
    {
        megaman->jumpTriggered = false;
    }

    return zPressed && megaman->isOnFloor && !megaman->jumpTriggered;
}

bool onMegamanLandTrigger(void *self)
{
    return isOnFloor(AS_ENTITY_PTR(self));
}

void onMegamanShoot(void *self)
{
    Megaman *megaman = (Megaman *)self;
    megaman->shootTime = 0.3f;
}

void onMegamanJump(void *self)
{
    Megaman *megaman = (Megaman *)self;
    megaman->speed.y = megaman->jumpStrength;
    megaman->jumpTriggered = true;
}

void onMegamanLand(void *self)
{
    Megaman *megaman = (Megaman *)self;
    megaman->speed.y = 0.0f;
    megaman->isOnFloor = true;
}

bool onMegamanCeilTrigger(void *self)
{
    return isOnCeil(AS_ENTITY_PTR(self));
}

void onMegamanCeil(void *self)
{
    Megaman *megaman = (Megaman *)self;
    megaman->speed.y = 0.0f;
}