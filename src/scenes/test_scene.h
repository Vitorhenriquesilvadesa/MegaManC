#ifndef TEST_SCENE_H
#define TEST_SCENE_H

#include <scene.h>

Scene *getTestScene();
Entity *newBrick(vec2s position);
void onUpdateBrick(void *self, float dt);
void onCollisionBrick(void *self, AABBColisionData data);

#endif