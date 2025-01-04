#include <test_scene.h>
#include <megaman/megaman.h>
#include <sizes.h>
#include <game.h>
#include <graphics_api.h>

Scene *getTestScene()
{
    Scene *scene = newScene();

    int bricks[14][16] = {
        {53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53},
        {25, 26, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 40},
        {25, 26, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 40},
        {25, 26, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 40},
        {25, 26, 52, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 40},
        {25, 26, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 40},
        {25, 26, 00, 00, 00, 00, 16, 17, 18, 19, 20, 00, 00, 00, 00, 40},
        {25, 26, 16, 20, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 40},
        {25, 26, 25, 26, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 40},
        {25, 26, 25, 26, 00, 00, 00, 00, 00, 00, 00, 16, 17, 19, 20, 40},
        {25, 26, 16, 17, 18, 18, 18, 18, 18, 19, 20, 53, 53, 53, 53, 40},
        {33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33},
        {41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41},
        {53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53},
    };

    for (int i = 0; i < 14; i++)
    {
        for (int j = 0; j < 16; j++)
        {
            if (bricks[i][j])
            {
                addTileToScene(scene, newBrick((vec2s){j * 16 - 128, i * 16 - 112}, bricks[i][j]));
            }
        }
    }

    generateColliders(scene, bricks, 14, 16);

    addObjectToScene(scene, AS_ENTITY_PTR(newMegaman((vec2s){0.0f, -64.0f})));

    return scene;
}

Tile newBrick(vec2s position, int id)
{
    static TilemapAtlas *atlas = NULL;

    if (!atlas)
    {
        atlas = newTilemapAtlasFromImage("../assets/tilemap/tilemap_001.png", (vec2s){16.0f, 16.0f});
    }

    Tile tile = getTileFromAtlas(atlas, id, position);

    return tile;
}

Entity *newCollider(vec2s position, vec2s size)
{
    static SpriteRenderer *renderer;

    if (!renderer)
    {
        GraphicsAPI *graphics = (GraphicsAPI *)getGameInstanceService(SERVICE_TYPE_GRAPHICS);

        Shader *shader = getShader(graphics, SHADER_TYPE_SPRITE);
        Texture *texture = newTextureFromImage("../assets/sprites/null/null.png");
        Animation *animation = newAnimation(01, 01, texture, false, PLAY_FROM_BEGIN);
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
            if (bricks[i][j] >= 1 && !visited[i][j])
            {
                int startX = j, startY = i;

                int endX = startX;
                while (endX + 1 < cols && bricks[i][endX + 1] >= 1 && !visited[i][endX + 1])
                {
                    endX++;
                }

                int endY = startY;
                bool validColumn = true;
                while (validColumn && endY + 1 < rows)
                {
                    for (int x = startX; x <= endX; x++)
                    {
                        if (!(bricks[endY + 1][x]) || visited[endY + 1][x])
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
