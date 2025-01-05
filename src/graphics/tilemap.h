#ifndef TILEMAP_H
#define TILEMAP_H

#include <texture.h>

#define CGLM_USE_STRUCT
#include <cglm/struct.h>

typedef struct Camera2D Camera2D;
typedef struct Scene Scene;

typedef struct
{
    int rowCount;
    int columnCount;
    Texture *atlas;
    vec2s tileSize;
} TilemapAtlas;

typedef struct
{
    int id;
    vec2s position;
} Tile;

typedef struct
{
    size_t tileCount;
    size_t width;
    size_t height;
    Tile *tiles;
    TilemapAtlas *atlas;
} TilemapData;

typedef struct
{
    int **bits;
} TilemapColliderMask;

TilemapAtlas *newTilemapAtlasFromImage(const char *filepath, vec2s tileSize);
Tile getTile(int id, vec2s position);
TilemapData *newTilemapData(size_t tileCount, size_t width, size_t height, Tile *tilemap, TilemapAtlas *atlas);
bool isTileOnScreen(Tile tile, vec2s tileSize, Camera2D *camera);
void generateColliders(Scene *scene, TilemapColliderMask *mask, int cols, int rows);
TilemapData *loadTilemapFromFiles(const char *tilemapFilepath, const char *sceneFilepath, TilemapColliderMask *mask);

#endif