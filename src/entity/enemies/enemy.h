#ifndef ENEMY_H
#define ENEMY_H

#include <entity.h>

#define METAL_MAX_HP 1

typedef enum
{
    ENEMY_TYPE_METAL,
} EnemyType;

typedef struct
{
    Entity entity;
    short life;
    EnemyType type;
} Enemy;

void initEnemy(Enemy *enemy, Entity entity, EnemyType type, short life);

#endif