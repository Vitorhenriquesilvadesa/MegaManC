#ifndef MEGAMAN_H
#define MEGAMAN_H

#include <entity.h>

typedef struct
{
    Animation *idle;
    Animation *walk;
    Animation *jump;
} MegamanResources;

typedef enum
{
    MEGAMAN_STATE_IDLE,
} MegamanState;

typedef struct
{
    Entity entity;
    MegamanState state;
    vec2s speed;
    float gravity;
    float maxSpeed;
    float maxFallSpeed;
    float jumpStrength;
    bool isJumping;
    bool isFalling;
    bool isMoving;
    bool isOnFloor;
    bool isLeftWall;
    bool isRightWall;
    bool isOnCeil;
} Megaman;

Megaman *newMegaman(vec2s position);

void onUpdateMegaman(void *self, float dt);
void onCollisionMegaman(void *self, AABBColisionData data);

#endif