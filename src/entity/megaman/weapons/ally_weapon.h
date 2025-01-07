#ifndef ALLY_WEAPON_H
#define ALLY_WEAPON_H

#include <entity.h>

typedef enum
{
    ALLY_WEAPON_MEGA_BUSTER,
} AllyWeaponType;

typedef struct
{
    Entity entity;
    AllyWeaponType type;
    short damage;
} AllyWeapon;

void initAllyWeapon(AllyWeapon *weapon, Entity entity, AllyWeaponType type, short damage);

#endif