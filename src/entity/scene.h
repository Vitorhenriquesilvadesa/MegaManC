#ifndef SCENE_H
#define SCENE_H

#include <entity.h>
#include <stdint.h>
#include <tilemap.h>

#define CGLM_USE_STRUCT
#include <cglm/struct.h>

typedef struct Scene
{
    Entity **entities;
    TilemapData *tilemap;
    vec2s size;
    uint32_t entityCount;
    uint32_t entityCapacity;
    void *metadata;
} Scene;

Scene *newScene(TilemapData *tilemap, void *metadata);
void addObjectToScene(Scene *scene, Entity *entity);

#endif