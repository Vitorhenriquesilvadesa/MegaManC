#include <test_scene.h>
#include <megaman/megaman.h>
#include <sizes.h>
#include <game.h>
#include <graphics_api.h>
#include <allocator.h>

Scene *getTestScene()
{
    static TilemapAtlas *atlas = NULL;

    if (!atlas)
    {
        atlas = newTilemapAtlasFromImage("../assets/tilemap/tilemap_001.png", (vec2s){16.0f, 16.0f});
    }

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

    size_t tileCount = 0;

    for (int i = 0; i < 14; i++)
    {
        for (int j = 0; j < 16; j++)
        {
            if (bricks[i][j])
            {
                tileCount++;
            }
        }
    }

    Tile *tiles = ALLOCATE(Tile, tileCount);

    size_t index = 0;

    for (int i = 0; i < 14; i++)
    {
        for (int j = 0; j < 16; j++)
        {
            if (bricks[i][j])
            {
                tiles[index++] = newBrick((vec2s){j * 16 - 128, i * 16 - 112}, bricks[i][j]);
            }
        }
    }

    // TilemapData *tilemap = newTilemapData(tileCount, tiles, atlas);

    TilemapColliderMask mask;
    TilemapData *tilemap = loadTilemapFromFiles("../assets/scenes/Tilemap001.tsj",
                                                "../assets/scenes/scene001.tmj",
                                                &mask);

    Scene *scene = newScene(tilemap);

    generateColliders(scene, &mask, tilemap->width, tilemap->height);

    addObjectToScene(scene, AS_ENTITY_PTR(newMegaman((vec2s){0.0f, -64.0f})));

    return scene;
}

Tile newBrick(vec2s position, int id)
{
    Tile tile = getTile(id, position);
    return tile;
}

void onUpdateBrick(void *self, float dt)
{
}

void onCollisionBrick(void *self, AABBColisionData data)
{
}
