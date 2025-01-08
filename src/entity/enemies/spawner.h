#ifndef ENEMY_SPAWNER_H
#define ENEMY_SPAWNER_H

#define CGLM_USE_STRUCT
#include <cglm/struct.h>

typedef struct Entity Entity;
typedef Entity *(*SpawnFn)(vec2s position);

typedef struct
{
    SpawnFn onSpawn;
    vec2s position;
    vec2s size;
    Entity *entity;
    bool canSpawn;
} EnemySpawner;

EnemySpawner *newEnemySpawner(SpawnFn onSpawn, vec2s position, vec2s size);

bool enemyCanSpawnEnemyTrigger(void *self);
void onSpawnerSpawnEnemy(void *self);
void updateSpawner(EnemySpawner *spawner);
bool isSpawnerOnScreen(EnemySpawner *spawner);

#endif