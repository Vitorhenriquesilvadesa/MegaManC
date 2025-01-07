#include <vfx.h>

void initVfx(Vfx *vfx, Entity entity, VfxType type, float lifeTime)
{
    vfx->type = type;
    vfx->entity = entity;
    vfx->lifeTime = lifeTime;
}