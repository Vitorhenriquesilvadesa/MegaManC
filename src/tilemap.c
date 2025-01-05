#include <tilemap.h>
#include <allocator.h>
#include <renderer.h>
#include <cJSON.h>
#include <files.h>
#include <string.h>

TilemapAtlas *newTilemapAtlasFromImage(const char *filepath, vec2s tileSize)
{
    TilemapAtlas *atlas = ALLOCATE(TilemapAtlas, 1);

    Texture *texture = newTextureFromImage(filepath);

    atlas->atlas = texture;
    atlas->rowCount = (int)(texture->height / tileSize.y);
    atlas->columnCount = (int)(texture->width / tileSize.x);
    atlas->tileSize = tileSize;

    return atlas;
}

Tile getTile(int id, vec2s position)
{
    Tile tile;
    tile.position = position;
    tile.id = id;

    return tile;
}

TilemapData *newTilemapData(size_t tileCount, size_t width, size_t height, Tile *tilemap, TilemapAtlas *atlas)
{
    TilemapData *data = ALLOCATE(TilemapData, 1);
    data->tileCount = tileCount;
    data->tiles = tilemap;
    data->atlas = atlas;
    data->width = width;
    data->height = height;

    return data;
}

bool isTileOnScreen(Tile tile, vec2s tileSize, Camera2D *camera)
{
    vec2s pos = tile.position;
    vec2s camPos = camera->position;
    vec2s scale = tileSize;

    return (
        pos.x > camPos.x - 128 - scale.x / 2.0f &&
        pos.x < camPos.x + 128 + scale.x / 2.0f &&
        pos.y > camPos.y - 112 - scale.y / 2.0f &&
        pos.y < camPos.y + 128 + scale.y / 2.0f);
}

void generateColliders(Scene *scene, TilemapColliderMask *mask, int cols, int rows)
{
    int **visited = ALLOCATE(int *, rows);

    for (int i = 0; i < rows; i++)
    {
        visited[i] = ALLOCATE(int, cols);
        memset(visited[i], 0, sizeof(int) * cols);
    }

    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            if (mask->bits[i][j] >= 1 && !visited[i][j])
            {
                int startX = j, startY = i;

                int endX = startX;
                while (endX + 1 < cols && mask->bits[i][endX + 1] >= 1 && !visited[i][endX + 1])
                {
                    endX++;
                }

                int endY = startY;
                bool validColumn = true;
                while (validColumn && endY + 1 < rows)
                {
                    for (int x = startX; x <= endX; x++)
                    {
                        if (!(mask->bits[endY + 1][x]) || visited[endY + 1][x])
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

                int tileSize = scene->tilemap->atlas->tileSize.x;
                vec2s size = {(endX - startX + 1) * tileSize, (endY - startY + 1) * tileSize};
                vec2s position = {startX * tileSize - 128 + size.x / 2.0f,
                                  startY * tileSize - 112 + size.y / 2.0f};

                addObjectToScene(scene, AS_ENTITY_PTR(newCollider(position, size)));
            }
        }
    }
}

TilemapData *loadTilemapFromFiles(const char *tilemapFilepath, const char *sceneFilepath, TilemapColliderMask *mask)
{
    const char *tilemapContent = readFile(tilemapFilepath);
    if (tilemapContent == NULL)
    {
        printf("Error to read tileset file: %s\n", tilemapFilepath);
        return NULL;
    }

    cJSON *tilemapRoot = cJSON_Parse(tilemapContent);
    if (tilemapRoot == NULL)
    {
        printf("Error to parse tileset file: %s\n", cJSON_GetErrorPtr());
        return NULL;
    }

    const char *imagePath = cJSON_GetObjectItem(tilemapRoot, "image")->valuestring;
    int tileWidth = cJSON_GetObjectItem(tilemapRoot, "tilewidth")->valueint;
    int tileHeight = cJSON_GetObjectItem(tilemapRoot, "tileheight")->valueint;

    vec2s tileSize = {tileWidth, tileHeight};
    TilemapAtlas *atlas = newTilemapAtlasFromImage(imagePath, tileSize);

    cJSON_Delete(tilemapRoot);

    const char *sceneContent = readFile(sceneFilepath);
    if (sceneContent == NULL)
    {
        printf("Error to read scene file: %s\n", sceneFilepath);
        return NULL;
    }

    cJSON *sceneRoot = cJSON_Parse(sceneContent);
    if (sceneRoot == NULL)
    {
        printf("Error to parse scene file: %s\n", cJSON_GetErrorPtr());
        return NULL;
    }

    cJSON *layers = cJSON_GetObjectItem(sceneRoot, "layers");
    if (layers == NULL)
    {
        printf("Error: Cannot find 'layers' tag in JSON.\n");
        cJSON_Delete(sceneRoot);
        return NULL;
    }

    cJSON *layer = cJSON_GetArrayItem(layers, 0);
    if (layer == NULL)
    {
        printf("Error: layer not found.\n");
        cJSON_Delete(sceneRoot);
        return NULL;
    }

    cJSON *data = cJSON_GetObjectItem(layer, "data");
    if (data == NULL)
    {
        printf("Error: Cannot find 'data' tag in layer.\n");
        cJSON_Delete(sceneRoot);
        return NULL;
    }

    size_t tileCount = cJSON_GetArraySize(data);
    Tile *tiles = ALLOCATE(Tile, tileCount);
    if (tiles == NULL)
    {
        cJSON_Delete(sceneRoot);
        return NULL;
    }

    size_t validTileCount = 0;

    int mapWidth = cJSON_GetObjectItem(layer, "width")->valueint;
    int mapHeight = cJSON_GetObjectItem(layer, "height")->valueint;

    mask->bits = ALLOCATE(int *, mapHeight);

    if (mask->bits == NULL)
    {
        cJSON_Delete(sceneRoot);
        return NULL;
    }

    for (int i = 0; i < mapHeight; i++)
    {
        mask->bits[i] = ALLOCATE(int, mapWidth);
        if (mask->bits[i] == NULL)
        {
            cJSON_Delete(sceneRoot);
            return NULL;
        }
        memset(mask->bits[i], 0, sizeof(int) * mapWidth);
    }

    for (size_t i = 0; i < tileCount; i++)
    {
        int tileId = cJSON_GetArrayItem(data, i)->valueint;

        if (tileId != 0)
        {
            int x = i % mapWidth;
            int y = i / mapWidth;

            mask->bits[y][x] = 1;

            float tileX = x * atlas->tileSize.x - 128;
            float tileY = y * atlas->tileSize.y - 112;

            tiles[validTileCount] = getTile(tileId - 1, (vec2s){tileX, tileY});
            validTileCount++;
        }
    }

    Tile *validTiles = ALLOCATE(Tile, validTileCount);
    if (validTiles == NULL)
    {
        cJSON_Delete(sceneRoot);
        return NULL;
    }

    for (size_t i = 0; i < validTileCount; i++)
    {
        validTiles[i] = tiles[i];
    }

    TilemapData *tilemapData = newTilemapData(validTileCount, mapWidth, mapHeight, validTiles, atlas);

    cJSON_Delete(sceneRoot);

    return tilemapData;
}
