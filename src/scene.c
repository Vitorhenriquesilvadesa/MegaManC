#include <scene.h>
#include <allocator.h>

Scene *newScene(TilemapData *tilemap, void *metadata)
{
    Scene *scene = ALLOCATE(Scene, 1);
    scene->entities = ALLOCATE(Entity *, ARRAY_MIN_SIZE);
    scene->spawners = ALLOCATE(EnemySpawner *, ARRAY_MIN_SIZE);
    scene->entityCapacity = ARRAY_MIN_SIZE;
    scene->entityCount = 0;
    scene->spawnerCapacity = ARRAY_MIN_SIZE;
    scene->spawnerCount = 0;
    scene->tilemap = tilemap;
    scene->size.x = tilemap->width * tilemap->atlas->tileSize.x;
    scene->size.y = tilemap->height * tilemap->atlas->tileSize.y;
    scene->metadata = metadata;

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

    entity->index = scene->entityCount;
    scene->entities[scene->entityCount++] = entity;
}

void addSpawnerToScene(Scene *scene, EnemySpawner *spawner)
{
    if (scene->spawnerCount >= scene->spawnerCapacity - 1)
    {
        uint32_t newCapacity = scene->spawnerCapacity * ARRAY_GROW_FACTOR;
        scene->spawners = REALLOCATE(EnemySpawner *, scene->spawners, newCapacity);
        scene->spawnerCapacity = newCapacity;
    }

    scene->spawners[scene->spawnerCount++] = spawner;
}
