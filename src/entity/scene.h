#ifndef SCENE_H
#define SCENE_H

#include <entity.h>
#include <stdint.h>
#include <tilemap.h>

typedef struct Scene
{
    Entity **entities;
    TilemapData *tilemap;
    uint32_t entityCount;
    uint32_t entityCapacity;
} Scene;

Scene *newScene(TilemapData *tilemap);
void addObjectToScene(Scene *scene, Entity *entity);

#endif