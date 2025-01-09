#ifndef LIFE_BAR_H
#define LIFE_BAR_H

#include <entity.h>

#define CGLM_USE_STRUCT
#include <cglm/struct.h>

typedef struct
{
    Entity entity;
    vec2s spawnPosition;
    vec2s spawnSize;
    float value;
} LifeBar;

typedef struct
{
    Entity entity;
} LifeBarBG;

LifeBar *newLifebar(vec2s position);
LifeBarBG *newLifebarBG(vec2s position);

void onUpdateLifeBar(void *self, float dt);
void onCollisionLifeBar(void *self, AABBColisionData data);

#endif