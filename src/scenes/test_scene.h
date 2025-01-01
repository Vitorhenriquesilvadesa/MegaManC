#ifndef TEST_SCENE_H
#define TEST_SCENE_H

#include <scene.h>

Scene *getTestScene();
Entity *newBrick(vec2s position);
void updateBrick(void *self, float dt);

#endif