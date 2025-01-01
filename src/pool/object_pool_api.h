#ifndef OBJECT_POOL_H
#define OBJECT_POOL_H

#include <service.h>
#include <scene.h>

typedef struct
{
    Service service;
    Scene *scene;
} ObjectPoolAPI;

ObjectPoolAPI *newObjectPoolAPI();

void initObjectPoolAPI(void *self);
void updateObjectPoolAPI(void *self, float dt);
void shutdownObjectPoolAPI(void *self);

void updateScene(Scene *scene, float dt);

#endif