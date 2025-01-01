#ifndef ANIMATION_H
#define ANIMATION_H

#include <texture.h>
#include <stdint.h>

typedef struct
{
    uint32_t frameCount;
    uint32_t currentFrame;
    uint32_t speed;
    float elapsedTime;
    Texture *texture;
} Animation;

Animation *newAnimation(uint32_t frameCount, uint32_t speed, Texture *texture);
void updateAnimation(Animation *animation, float dt);

#endif