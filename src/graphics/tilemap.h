#ifndef TILEMAP_H
#define TILEMAP_H

#include <texture.h>

#define CGLM_USE_STRUCT
#include <cglm/struct.h>

typedef struct Camera2D Camera2D;

typedef struct
{
    int rowCount;
    int columnCount;
    Texture *atlas;
    vec2s tileSize;
} TilemapAtlas;

typedef struct
{
    TilemapAtlas *atlas;
    int id;
    vec2s position;
    vec2s size;
} Tile;

TilemapAtlas *newTilemapAtlasFromImage(const char *filepath, vec2s tileSize);
Tile getTileFromAtlas(TilemapAtlas *atlas, int id, vec2s position);
bool isTileOnScreen(Tile tile, Camera2D *camera);

#endif