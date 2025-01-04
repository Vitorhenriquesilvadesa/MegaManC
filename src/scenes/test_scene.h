#ifndef TEST_SCENE_H
#define TEST_SCENE_H

#include <scene.h>

Scene *getTestScene();
Tile newBrick(vec2s position, int id);
Entity *newCollider(vec2s position, vec2s size);
void onUpdateBrick(void *self, float dt);
void onUpdateNull(void *self, float dt);
void onCollisionBrick(void *self, AABBColisionData data);
void onCollisionNull(void *self, AABBColisionData data);
void generateColliders(Scene *scene, int bricks[14][16], int rows, int cols);

#endif