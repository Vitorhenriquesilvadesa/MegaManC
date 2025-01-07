#ifndef ENEMY_METAL_H
#define ENEMY_METAL_H

#include <entity.h>
#include <stdbool.h>
#include <enemies/enemy.h>

#define CGLM_USE_STRUCT
#include <cglm/struct.h>

typedef enum
{
    METAL_STATE_ATTACK,
    METAL_STATE_DEFEND,
    METAL_STATE_MAX,
} MetalState;

typedef struct
{
    Animation *animations;
} MetalResources;

typedef struct
{
    Enemy enemy;
    MetalState state;
    float defendTimer;
    float attackTimer;
} Metal;

Metal *newMetal(vec2s position);
void onUpdateMetal(void *self, float dt);
void onCollisionMetal(void *self, AABBColisionData data);

bool onCollisionWithMegamanTrigger(void *self);
void onCollisionWithMegaman(void *self);

bool onMetalAttackTrigger(void *self);
void onMetalAttack(void *self);

#endif