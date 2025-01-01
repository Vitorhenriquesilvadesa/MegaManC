#include <megaman/megaman.h>
#include <sizes.h>
#include <game.h>
#include <graphics_api.h>
#include <allocator.h>
#include <GLFW/glfw3.h>

Animation *idle = NULL;
Animation *walk = NULL;

static void createAnimations()
{
    if (!walk)
    {
        Texture *texture = newTextureFromImage("../assets/sprites/megaman/walk.png");
        walk = newAnimation(4, 10, texture);
    }

    if (!idle)
    {
        Texture *texture = newTextureFromImage("../assets/sprites/megaman/idle.png");
        idle = newAnimation(15, 10, texture);
    }
}

Megaman *newMegaman(vec2s position)
{
    GraphicsAPI *graphics = (GraphicsAPI *)getGameInstanceService(SERVICE_TYPE_GRAPHICS);

    Shader *shader = getShader(graphics, SHADER_TYPE_SPRITE);

    createAnimations();

    SpriteRenderer *renderer = newSpriteRenderer(shader, idle);
    Entity entity;

    initEntity(&entity, ENTITY_TYPE_MEGAMAN, updateMegaman, position, MEGAMAN_SIZE, renderer);

    Megaman *megaman = ALLOCATE(Megaman, 1);
    megaman->entity = entity;

    return megaman;
}
void updateMegaman(void *self, float dt)
{
    Megaman *megaman = (Megaman *)self;

    if (isKeyPressed(GLFW_KEY_RIGHT))
    {
        megaman->entity.position.x += 90.0f * dt;
        megaman->entity.isMirrored = false;
        megaman->entity.renderer->currentAnimation = walk;
    }

    if (isKeyPressed(GLFW_KEY_LEFT))
    {
        megaman->entity.position.x -= 90.0f * dt;
        megaman->entity.isMirrored = true;
        megaman->entity.renderer->currentAnimation = walk;
    }

    if (!isKeyPressed(GLFW_KEY_LEFT) && !isKeyPressed(GLFW_KEY_RIGHT))
    {
        megaman->entity.renderer->currentAnimation = idle;
    }
}