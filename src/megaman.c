#include <megaman/megaman.h>
#include <sizes.h>
#include <game.h>
#include <graphics_api.h>
#include <allocator.h>
#include <GLFW/glfw3.h>

#define MEGAMAN_SPEED 90.0f

static MegamanResources megamanResources = {NULL, NULL};

static void createAnimations()
{
    if (!megamanResources.walk)
    {
        Texture *texture = newTextureFromImage("../assets/sprites/megaman/walk.png");
        megamanResources.walk = newAnimation(4, 10, texture, true, PLAY_FROM_BEGIN);
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

    initEntity(&entity, ENTITY_TYPE_MEGAMAN, updateMegaman, position, MEGAMAN_SIZE, renderer);

    Megaman *megaman = ALLOCATE(Megaman, 1);
    megaman->entity = entity;

    return megaman;
}
void updateMegaman(void *self, float dt)
{
    Megaman *megaman = (Megaman *)self;

    GraphicsAPI *graphics = (GraphicsAPI *)getGameInstanceService(SERVICE_TYPE_GRAPHICS);

    graphics->renderer->camera->position = megaman->entity.position;

    float resultantSpeed = 0.0f;

    resultantSpeed += MEGAMAN_SPEED * -isKeyPressed(GLFW_KEY_LEFT) + MEGAMAN_SPEED * isKeyPressed(GLFW_KEY_RIGHT);

    if (resultantSpeed == 0.0f)
    {
        megaman->entity.renderer->currentAnimation = megamanResources.idle;
    }
    else
    {
        megaman->entity.position.x += resultantSpeed * dt;
        if (megaman->entity.renderer->currentAnimation != megamanResources.walk)
        {
            megamanResources.walk->currentFrame = 0;
            megamanResources.walk->elapsedTime = 0.0f;
            megaman->entity.renderer->currentAnimation = megamanResources.walk;
        }

        megaman->entity.isMirrored = resultantSpeed < 0.0f;
    }
}