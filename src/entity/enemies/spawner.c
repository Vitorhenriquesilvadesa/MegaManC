#include <enemies/spawner.h>
#include <trigger.h>
#include <game.h>
#include <allocator.h>
#include <graphics_api.h>
#include "spawner.h"

EnemySpawner *newEnemySpawner(SpawnFn onSpawn, vec2s position, vec2s size)
{
    EnemySpawner *spawner = ALLOCATE(EnemySpawner, 1);
    spawner->onSpawn = onSpawn;
    spawner->position = position;
    spawner->size = size;
    spawner->canSpawn = true;
    spawner->entity = NULL;

    TriggerAPI *triggers = (TriggerAPI *)getGameInstanceService(SERVICE_TYPE_EVENT);

    static Id currentId = (1 << (sizeof(Id) / 8));

    registerTrigger(triggers, currentId--, enemyCanSpawnEnemyTrigger, onSpawnerSpawnEnemy, spawner);

    return spawner;
}

bool enemyCanSpawnEnemyTrigger(void *self)
{
    EnemySpawner *spawner = (EnemySpawner *)self;
    return isSpawnerOnScreen(spawner) && spawner->entity == NULL && spawner->canSpawn;
}

void onSpawnerSpawnEnemy(void *self)
{
    EnemySpawner *spawner = (EnemySpawner *)self;
    Entity *entity = AS_ENTITY_PTR(spawner->onSpawn(spawner->position));
    Scene *scene = getGameInstanceActiveScene();
    addObjectToScene(scene, entity);
    spawner->canSpawn = false;
    spawner->entity = entity;
}

void updateSpawner(EnemySpawner *spawner)
{
    if (spawner->entity != NULL && !spawner->entity->isEnabled)
    {
        spawner->entity = NULL;
    }

    if (!isSpawnerOnScreen(spawner) && !spawner->canSpawn)
    {
        spawner->canSpawn = true;
    }
}

bool isSpawnerOnScreen(EnemySpawner *spawner)
{
    Camera2D *camera = ((GraphicsAPI *)getGameInstanceService(SERVICE_TYPE_GRAPHICS))->renderer->camera;

    vec2s pos = spawner->position;
    vec2s camPos = camera->position;
    vec2s scale = spawner->size;

    return (
        pos.x > camPos.x - 128 - scale.x / 2.0f &&
        pos.x < camPos.x + 128 + scale.x / 2.0f &&
        pos.y > camPos.y - 112 - scale.y / 2.0f &&
        pos.y < camPos.y + 128 + scale.y / 2.0f);
}
