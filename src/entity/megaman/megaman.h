#ifndef MEGAMAN_H
#define MEGAMAN_H

#include <entity.h>

typedef enum
{
    MEGAMAN_STATE_IDLE,
    MEGAMAN_STATE_WALK,
    MEGAMAN_STATE_JUMP,
    MEGAMAN_STATE_MAX,
} MegamanState;

typedef struct
{
    Animation *normalAnimations;
    Animation *shootingAnimations;
} MegamanResources;

typedef struct
{
    Entity entity;
    MegamanState state;
    vec2s speed;
    float gravity;
    float maxSpeed;
    float maxFallSpeed;
    float jumpStrength;
    float shootTime;
    float megaBusterTimer;
    bool shootExceeded;
    bool isJumping;
    bool isFalling;
    bool isMoving;
    bool isOnFloor;
    bool isLeftWall;
    bool isRightWall;
    bool isOnCeil;
    bool jumpTriggered;
} Megaman;

Megaman *newMegaman(vec2s position);

void onUpdateMegaman(void *self, float dt);
void onCollisionMegaman(void *self, AABBColisionData data);

bool onMegamanShootTrigger(void *self);
void onMegamanShoot(void *self);

bool onMegamanJumpTrigger(void *self);
void onMegamanJump(void *self);

bool onMegamanLandTrigger(void *self);
void onMegamanLand(void *self);

bool onMegamanCeilTrigger(void *self);
void onMegamanCeil(void *self);

#endif