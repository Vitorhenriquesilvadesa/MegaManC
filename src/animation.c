#include <animation.h>
#include <allocator.h>

Animation newAnimation(uint32_t frameCount, uint32_t speed, Texture *texture, bool isLooping, AnimationPlay play)
{
    Animation animation;

    animation.currentFrame = 0;
    animation.elapsedTime = 0.0f;
    animation.frameCount = frameCount;
    animation.texture = texture;
    animation.speed = speed;
    animation.isLooping = isLooping;
    animation.play = play;

    return animation;
}

void updateAnimation(Animation *animation, float dt)
{
    animation->elapsedTime += dt;

    float frameTime = 1.0f / animation->speed;

    if (animation->elapsedTime >= frameTime)
    {
        if (animation->currentFrame == animation->frameCount - 1)
        {
            if (animation->isLooping)
            {
                animation->currentFrame = 0;
            }
        }
        else
        {
            animation->currentFrame++;
        }

        animation->elapsedTime = 0.0f;
    }
}