#include <enemies/enemy.h>

void initEnemy(Enemy *enemy, Entity entity, EnemyType type, short life)
{
    enemy->entity = entity;
    enemy->type = type;
    enemy->life = life;
}