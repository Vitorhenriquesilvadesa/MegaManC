#ifndef MEGAMAN_H
#define MEGAMAN_H

#include <entity.h>

typedef struct
{
    Entity entity;
} Megaman;

Megaman *newMegaman(vec2s position);

void updateMegaman(void *self, float dt);

#endif