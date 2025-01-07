#ifndef ALLY_NORMAL_SHOOT_H
#define ALLY_NORMAL_SHOOT_H

#include <aabb.h>
#include <megaman/weapons/ally_weapon.h>

#define CGLM_USE_STRUCT
#include <cglm/struct.h>

typedef struct
{
    AllyWeapon weapon;
    float speed;
    float angle;
} MegaBusterShoot;

MegaBusterShoot *newMegaBusterShoot(vec2s position, float angle);
void onUpdateMegaBusterShoot(void *self, float dt);
void onCollisionMegaBusterShoot(void *self, AABBColisionData data);

#endif