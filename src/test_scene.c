#include <test_scene.h>
#include <megaman/megaman.h>
#include <sizes.h>
#include <game.h>
#include <graphics_api.h>

Scene *getTestScene()
{
    Scene *scene = newScene();
    Megaman *megaman = newMegaman((vec2s){0.0f, 1.0f});
    addObjectToScene(scene, AS_ENTITY_PTR(megaman));

    for (int x = -128; x < 128; x += 16)
    {
        for (int y = 16; y < 32; y += 16)
        {
            addObjectToScene(scene, newBrick((vec2s){x, y}));
        }
    }

    return scene;
}

Entity *newBrick(vec2s position)
{
    static SpriteRenderer *renderer = NULL;

    if (!renderer)
    {
        GraphicsAPI *graphics = (GraphicsAPI *)getGameInstanceService(SERVICE_TYPE_GRAPHICS);
        Shader *shader = getShader(graphics, SHADER_TYPE_SPRITE);
        Texture *brick = newTextureFromImage("../assets/tilemap/brick_tile.png");
        Animation *animation = newAnimation(1, 1, brick, false, PLAY_FROM_BEGIN);
        renderer = newSpriteRenderer(shader, animation);
    }

    Entity *entity = newEntity(ENTITY_TYPE_BRICK, updateBrick, (vec2s){position.x + 8, position.y + 8}, TILE_SIZE, renderer);
    return entity;
}

void updateBrick(void *self, float dt)
{
}
