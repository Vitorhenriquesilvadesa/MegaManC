#ifndef SCENE_H
#define SCENE_H

#include <entity.h>
#include <stdint.h>

typedef struct
{
    Entity **entities;
    Entity **tilemap;
    uint32_t entityCount;
    uint32_t entityCapacity;
    uint32_t brickCount;
    uint32_t brickCapacity;
} Scene;

Scene *newScene();
void addObjectToScene(Scene *scene, Entity *entity);
void addBrickToScene(Scene *scene, Entity *entity);

#endif