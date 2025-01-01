#ifndef SCENE_H
#define SCENE_H

#include <entity.h>
#include <stdint.h>

typedef struct
{
    Entity **entities;
    uint32_t entityCount;
    uint32_t entityCapacity;
} Scene;

Scene *newScene();
void addObjectToScene(Scene *scene, Entity *entity);

#endif