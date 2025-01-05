#include <scene.h>
#include <allocator.h>

Scene *newScene(TilemapData *tilemap)
{
    Scene *scene = ALLOCATE(Scene, 1);
    scene->entities = ALLOCATE(Entity *, ARRAY_MIN_SIZE);
    scene->entityCapacity = ARRAY_MIN_SIZE;
    scene->entityCount = 0;
    scene->tilemap = tilemap;

    return scene;
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