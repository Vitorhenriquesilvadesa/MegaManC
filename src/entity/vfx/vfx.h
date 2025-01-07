#ifndef VFX_H
#define VFX_H

#include <entity.h>

typedef enum
{
    VFX_SMALL_EXPLOSION,
} VfxType;

typedef struct
{
    Entity entity;
    VfxType type;
    float lifeTime;
} Vfx;

void initVfx(Vfx *vfx, Entity entity, VfxType type, float lifeTime);

#endif