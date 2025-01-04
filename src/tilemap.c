#include <tilemap.h>
#include <allocator.h>
#include <renderer.h>

TilemapAtlas *newTilemapAtlasFromImage(const char *filepath, vec2s tileSize)
{
    TilemapAtlas *atlas = ALLOCATE(TilemapAtlas, 1);

    Texture *texture = newTextureFromImage(filepath);

    atlas->atlas = texture;
    atlas->rowCount = (int)(texture->height / tileSize.y);
    atlas->columnCount = (int)(texture->width / tileSize.x);
    atlas->tileSize = tileSize;

    // printf("Atlas row count: %d", atlas->rowCount);

    return atlas;
}

Tile getTileFromAtlas(TilemapAtlas *atlas, int id, vec2s position)
{
    Tile tile;
    tile.position = position;
    tile.atlas = atlas;
    tile.id = id;
    tile.size = atlas->tileSize;

    return tile;
}

bool isTileOnScreen(Tile tile, Camera2D *camera)
{
    vec2s pos = tile.position;
    vec2s camPos = camera->position;
    vec2s scale = tile.size;

    return (
        pos.x > camPos.x - 128 - scale.x / 2.0f &&
        pos.x < camPos.x + 128 + scale.x / 2.0f &&
        pos.y > camPos.y - 112 - scale.y / 2.0f &&
        pos.y < camPos.y + 128 + scale.y / 2.0f);
}
