#include <scene.h>
#include <allocator.h>

Scene *newScene()
{
    Scene *scene = ALLOCATE(Scene, 1);
    scene->entities = ALLOCATE(Entity *, ARRAY_MIN_SIZE);
    scene->entityCapacity = ARRAY_MIN_SIZE;
    scene->entityCount = 0;
    scene->tilemap = ALLOCATE(Entity *, ARRAY_MIN_SIZE);
    scene->brickCapacity = ARRAY_MIN_SIZE;
    scene->brickCount = 0;
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

void addBrickToScene(Scene *scene, Entity *entity)
{
    if (scene->brickCount >= scene->brickCapacity - 1)
    {
        uint32_t newCapacity = scene->brickCapacity * ARRAY_GROW_FACTOR;
        scene->tilemap = REALLOCATE(Entity *, scene->tilemap, newCapacity);
        scene->brickCapacity = newCapacity;
    }

    scene->tilemap[scene->brickCount++] = entity;
}