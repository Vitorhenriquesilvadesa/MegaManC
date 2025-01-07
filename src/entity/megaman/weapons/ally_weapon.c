#include <megaman/weapons/ally_weapon.h>
#include "ally_weapon.h"

void initAllyWeapon(AllyWeapon *weapon, Entity entity, AllyWeaponType type, short damage)
{
    weapon->entity = entity;
    weapon->type = type;
    weapon->damage = damage;
}