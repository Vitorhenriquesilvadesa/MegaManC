#ifndef SCENE_H
#define SCENE_H

#include <entity.h>
#include <stdint.h>
#include <tilemap.h>
#include <enemies/spawner.h>

#define CGLM_USE_STRUCT
#include <cglm/struct.h>

typedef struct Scene
{
    Entity **entities;
    Entity **hudElements;
    Entity **backgroundElements;
    EnemySpawner **spawners;
    TilemapData *tilemap;
    vec2s size;
    uint32_t entityCount;
    uint32_t entityCapacity;
    uint32_t spawnerCount;
    uint32_t spawnerCapacity;
    uint32_t hudCapacity;
    uint32_t hudCount;
    uint32_t backgroundCapacity;
    uint32_t backgroundCount;
    void *metadata;
} Scene;

Scene *newScene(TilemapData *tilemap, void *metadata);
void addObjectToScene(Scene *scene, Entity *entity);
void addObjectToBackground(Scene *scene, Entity *entity);
void addSpawnerToScene(Scene *scene, EnemySpawner *spawner);
void addObjectToHUD(Scene *scene, Entity *entity);

#endif