#ifndef MEGAMAN_H
#define MEGAMAN_H

#include <entity.h>

typedef struct
{
    Animation *idle;
    Animation *walk;
} MegamanResources;

typedef enum
{
    MEGAMAN_STATE_IDLE,
} MegamanState;

typedef struct
{
    Entity entity;
    MegamanState state;
} Megaman;

Megaman *newMegaman(vec2s position);

void updateMegaman(void *self, float dt);

#endif