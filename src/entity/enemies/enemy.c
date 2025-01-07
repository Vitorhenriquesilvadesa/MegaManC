#include <enemies/enemy.h>

void initEnemy(Enemy *enemy, Entity entity, EnemyType type)
{
    enemy->entity = entity;
    enemy->type = type;
}