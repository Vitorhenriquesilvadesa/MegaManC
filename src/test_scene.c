#include <test_scene.h>
#include <megaman/megaman.h>
#include <sizes.h>
#include <game.h>
#include <graphics_api.h>

Scene *getTestScene()
{
    Scene *scene = newScene();
    Megaman *megaman = newMegaman((vec2s){0.0f, -48.0f});
    addObjectToScene(scene, AS_ENTITY_PTR(megaman));

    int bricks[14][16] = {
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    };

    for (int i = 0; i < 14; i++)
    {
        for (int j = 0; j < 16; j++)
        {
            if (bricks[i][j])
            {
                addObjectToScene(scene, newBrick((vec2s){j * 16 - 128, i * 16 - 112}));
            }
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

    Entity *entity = newEntity(ENTITY_TYPE_BRICK, onUpdateBrick, onCollisionBrick, (vec2s){position.x + 8, position.y + 8},
                               TILE_SIZE, (vec2s){0.0f, 0.0f}, TILE_SIZE, true, renderer);
    return entity;
}

void onUpdateBrick(void *self, float dt)
{
}

void onCollisionBrick(void *self, AABBColisionData data)
{
}
