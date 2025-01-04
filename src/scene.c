#include <scene.h>
#include <allocator.h>

Scene *newScene()
{
    Scene *scene = ALLOCATE(Scene, 1);
    scene->entities = ALLOCATE(Entity *, ARRAY_MIN_SIZE);
    scene->entityCapacity = ARRAY_MIN_SIZE;
    scene->entityCount = 0;
    scene->tilemap = ALLOCATE(Tile, ARRAY_MIN_SIZE);
    scene->tileCapacity = ARRAY_MIN_SIZE;
    scene->tileCount = 0;
}

void addObjectToScene(Scene *scene, Entity *entity)
{
    if (scene->entityCount >= scene->entityCapacity - 1)
    {
        uint32_t newCapacity = scene->entityCapacity * ARRAY_GROW_FACTOR;
        scene->entities = REALLOCATE(Entity *, scene->entities, newCapacity);
        scene->entityCapacity = newCapacity;
    }

    scene->entities[scene->entityCount++] = entity;
}

void addTileToScene(Scene *scene, Tile tile)
{
    if (scene->tileCount >= scene->tileCapacity - 1)
    {
        uint32_t newCapacity = scene->tileCapacity * ARRAY_GROW_FACTOR;
        scene->tilemap = REALLOCATE(Tile, scene->tilemap, newCapacity);
        scene->tileCapacity = newCapacity;
    }

    scene->tilemap[scene->tileCount++] = tile;
}