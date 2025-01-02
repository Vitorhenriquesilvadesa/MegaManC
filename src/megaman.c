#include <megaman/megaman.h>
#include <sizes.h>
#include <game.h>
#include <object_pool_api.h>
#include <graphics_api.h>
#include <allocator.h>
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
               (vec2s){9.0f, 3.0f}, (vec2s){21.0f, 26.0f}, false, renderer);

    Megaman *megaman = ALLOCATE(Megaman, 1);
    megaman->entity = entity;
    megaman->maxFallSpeed = MEGAMAN_AIR_MAX_FALL_SPEED;
    megaman->gravity = MEGAMAN_GRAVITY;
    megaman->maxSpeed = MEGAMAN_MAX_SPEED;
    megaman->jumpStrength = MEGAMAN_AIR_JUMP_STRENGTH;
    megaman->speed = (vec2s){0.0f, 0.0f};

    return megaman;
}

static bool isOnFloor(Entity *entity);

void onUpdateMegaman(void *self, float dt)
{
    Megaman *megaman = (Megaman *)self;

    megaman->isOnFloor = isOnFloor(&megaman->entity);

    GraphicsAPI *graphics = (GraphicsAPI *)getGameInstanceService(SERVICE_TYPE_GRAPHICS);

    graphics->renderer->camera->position.x = megaman->entity.transform.position.x;

    megaman->speed.x = MEGAMAN_MAX_SPEED * -isKeyPressed(GLFW_KEY_LEFT) + MEGAMAN_MAX_SPEED * isKeyPressed(GLFW_KEY_RIGHT);
    megaman->isMoving = megaman->speed.x != 0.0f;

    if ((megaman->isLeftWall && megaman->speed.x < 0.0f) || (megaman->isRightWall && megaman->speed.x > 0.0f))
    {
        megaman->speed.x = 0.0f;
    }

    static bool jumped = false;

    if (isKeyPressed(GLFW_KEY_Z) && !jumped && megaman->isOnFloor && !megaman->isJumping)
    {
        jumped = true;
        megaman->isOnFloor = false;
        megaman->isFalling = false;
        megaman->isJumping = true;
        megaman->entity.transform.position.y -= 1.0f;
        megaman->speed.y = -megaman->jumpStrength;
        megaman->entity.renderer->currentAnimation = megamanResources.jump;
    }

    if (!isKeyPressed(GLFW_KEY_Z))
    {
        jumped = false;
    }

    if (megaman->speed.y != 0.0f && megaman->entity.renderer->currentAnimation != megamanResources.jump)
    {
        megaman->entity.renderer->currentAnimation = megamanResources.jump;
    }

    if (megaman->isOnCeil && megaman->speed.y < 0.0f)
    {
        megaman->speed.y = 0.0f;
    }

    if (!megaman->isOnFloor)
    {
        if (megaman->speed.y < megaman->maxFallSpeed)
        {
            megaman->speed.y += megaman->gravity * dt;
        }
    }
    else
    {
        if (megaman->speed.y > 0.0f)
        {
            megaman->speed.y = 0.0f;
            megaman->isJumping = false;
        }
    }

    if (megaman->isOnFloor)
    {
        if (!megaman->isMoving)
        {
            if (megaman->entity.renderer->currentAnimation != megamanResources.idle)
            {
                megaman->entity.renderer->currentAnimation = megamanResources.idle;
            }
        }
        else
        {
            if (megaman->entity.renderer->currentAnimation != megamanResources.walk)
            {
                megamanResources.walk->currentFrame = 0;
                megamanResources.walk->elapsedTime = 0.0f;
                megaman->entity.renderer->currentAnimation = megamanResources.walk;
            }
        }
    }

    if (isKeyPressed(GLFW_KEY_LEFT))
    {
        megaman->entity.isMirrored = true;
    }

    if (isKeyPressed(GLFW_KEY_RIGHT))
    {
        megaman->entity.isMirrored = false;
    }

    if (!isKeyPressed(GLFW_KEY_Z) && megaman->isJumping && !megaman->isFalling &&
        megaman->speed.y < 0.0f)
    {
        megaman->isFalling = true;
        megaman->speed.y = 0.0f;
    }

    megaman->entity.transform.position.x += megaman->speed.x * dt;
    megaman->entity.transform.position.y += megaman->speed.y * dt;

    megaman->isLeftWall = false;
    megaman->isRightWall = false;
}

static void updateInfo(Megaman *megaman, float verticalOffset)
{
    ObjectPoolAPI *pool = (ObjectPoolAPI *)getGameInstanceService(SERVICE_TYPE_OBJECT_POOL);
    Scene *scene = pool->scene;

    Entity *entity = &megaman->entity;

    vec2s entityPos = entity->transform.position;
    vec2s checkPos = {entityPos.x, entityPos.y - 1.0f};

    megaman->isOnFloor = false;
    megaman->isLeftWall = false;
    megaman->isRightWall = false;

    for (uint32_t i = 0; i < scene->entityCount; i++)
    {
        Entity *other = scene->entities[i];

        if (entity != other && AABBIntersect(entity, other))
        {
            AABB otherAABB = other->collider;
            vec2s otherPos = other->transform.position;

            if (fabsf(otherPos.y - entityPos.y) < verticalOffset && otherPos.x < entityPos.x)
            {
                megaman->isLeftWall = true;
            }

            if (fabsf(otherPos.y - entityPos.y) < verticalOffset && otherPos.x > entityPos.x)
            {
                megaman->isRightWall = true;
            }
        }
    }
}

static bool isOnFloor(Entity *entity)
{
    ObjectPoolAPI *pool = (ObjectPoolAPI *)getGameInstanceService(SERVICE_TYPE_OBJECT_POOL);
    Scene *scene = pool->scene;

    vec2s entityPos = entity->transform.position;
    vec2s checkPos = {entityPos.x, entityPos.y - 1.0f};

    for (uint32_t i = 0; i < scene->entityCount; i++)
    {
        Entity *other = scene->entities[i];

        if (entity != other && AABBIntersect(entity, other))
        {
            if (checkPos.y <= other->transform.position.y)
            {
                return true;
            }
        }
    }
    return false;
}

void onCollisionMegaman(void *self, AABBColisionData data)
{
    Megaman *megaman = (Megaman *)self;

    if (data.other->type == ENTITY_TYPE_BRICK)
    {
        bool up = (data.overlap & OVERLAP_UP) != 0;

        updateInfo(megaman, 14.0f);

        megaman->isOnCeil = up;

        bool floor = isOnFloor(&megaman->entity);
        megaman->isOnFloor = floor;

        if (floor && megaman->isJumping)
        {
            megaman->isJumping = false;
        }
    }
}