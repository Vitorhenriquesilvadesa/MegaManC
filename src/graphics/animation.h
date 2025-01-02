#ifndef ANIMATION_H
#define ANIMATION_H

#include <texture.h>
#include <stdint.h>
#include <stdbool.h>

typedef enum
{
    PLAY_FROM_BEGIN,
    PLAY_FROM_CURRENT_FRAME,
} AnimationPlay;

typedef struct
{
    uint32_t frameCount;
    uint32_t currentFrame;
    uint32_t speed;
    AnimationPlay play;
    bool isLooping;
    float elapsedTime;
    Texture *texture;
} Animation;

Animation *newAnimation(uint32_t frameCount, uint32_t speed, Texture *texture, bool isLooping, AnimationPlay play);
void updateAnimation(Animation *animation, float dt);

#endif