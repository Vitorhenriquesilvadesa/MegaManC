#include <test_scene.h>
#include <megaman/megaman.h>
#include <sizes.h>
#include <game.h>
#include <graphics_api.h>

Scene *getTestScene()
{
    Scene *scene = newScene();

    int bricks[14][16] = {
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 1, 0, 0, 1, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1},
        {1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1},
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
                addBrickToScene(scene, newBrick((vec2s){j * 16 - 128, i * 16 - 112}));
            }
        }
    }

    generateColliders(scene, bricks, 14, 16);

    addObjectToScene(scene, AS_ENTITY_PTR(newMegaman((vec2s){0.0f, -64.0f})));

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
                               TILE_SIZE, (vec2s){0.0f, 0.0f}, TILE_SIZE, true, false, renderer);
    return entity;
}

Entity *newCollider(vec2s position, vec2s size)
{
    static SpriteRenderer *renderer;

    if (!renderer)
    {
        GraphicsAPI *graphics = (GraphicsAPI *)getGameInstanceService(SERVICE_TYPE_GRAPHICS);

        Shader *shader = getShader(graphics, SHADER_TYPE_SPRITE);
        Texture *texture = newTextureFromImage("../assets/sprites/null/null.png");
        Animation *animation = newAnimation(1, 1, texture, false, PLAY_FROM_BEGIN);
        renderer = newSpriteRenderer(shader, animation);
    }

    vec2s halfSize = (vec2s){size.x / 2.0f, size.y / 2.0f};

    Entity *entity = newEntity(ENTITY_TYPE_COLLIDER, onUpdateNull, onCollisionNull,
                               position, size,
                               (vec2s){0.0f, 0.0f},
                               (vec2s){size.x, size.y},
                               true, false, renderer);

    return entity;
}

void onUpdateBrick(void *self, float dt)
{
}

void onUpdateNull(void *self, float dt)
{
}

void onCollisionBrick(void *self, AABBColisionData data)
{
}

void onCollisionNull(void *self, AABBColisionData data)
{
}

void generateColliders(Scene *scene, int bricks[14][16], int rows, int cols)
{
    int visited[14][16] = {0};

    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            if (bricks[i][j] == 1 && !visited[i][j])
            {
                int startX = j, startY = i;

                int endX = startX;
                while (endX + 1 < cols && bricks[i][endX + 1] == 1 && !visited[i][endX + 1])
                {
                    endX++;
                }

                int endY = startY;
                bool validColumn = true;
                while (validColumn && endY + 1 < rows)
                {
                    for (int x = startX; x <= endX; x++)
                    {
                        if (bricks[endY + 1][x] != 1 || visited[endY + 1][x])
                        {
                            validColumn = false;
                            break;
                        }
                    }
                    if (validColumn)
                    {
                        endY++;
                    }
                }

                for (int y = startY; y <= endY; y++)
                {
                    for (int x = startX; x <= endX; x++)
                    {
                        visited[y][x] = 1;
                    }
                }

                int tileSize = TILE_SIZE.x;
                vec2s size = {(endX - startX + 1) * tileSize, (endY - startY + 1) * tileSize};
                vec2s position = {startX * tileSize - 128 + size.x / 2.0f,
                                  startY * tileSize - 112 + size.y / 2.0f};

                addObjectToScene(scene, AS_ENTITY_PTR(newCollider(position, size)));
            }
        }
    }
}
