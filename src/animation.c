#include <animation.h>
#include <allocator.h>

Animation *newAnimation(uint32_t frameCount, uint32_t speed, Texture *texture)
{
    Animation *animation = ALLOCATE(Animation, 1);

    animation->currentFrame = 0;
    animation->elapsedTime = 0.0f;
    animation->frameCount = frameCount;
    animation->texture = texture;
    animation->speed = speed;

    return animation;
}

void updateAnimation(Animation *animation, float dt)
{
    animation->elapsedTime += dt;

    float frameTime = 1.0f / animation->speed;

    if (animation->elapsedTime >= frameTime)
    {
        animation->currentFrame = (animation->currentFrame + 1) % animation->frameCount;
        animation->elapsedTime = 0.0f;
    }
}