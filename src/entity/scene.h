#ifndef SCENE_H
#define SCENE_H

#include <entity.h>
#include <stdint.h>
#include <tilemap.h>

typedef struct
{
    Entity **entities;
    Tile *tilemap;
    uint32_t entityCount;
    uint32_t entityCapacity;
    uint32_t tileCount;
    uint32_t tileCapacity;
} Scene;

Scene *newScene();
void addObjectToScene(Scene *scene, Entity *entity);
void addTileToScene(Scene *scene, Tile tile);

#endif