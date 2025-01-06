#ifndef SPRITE_H
#define SPRITE_H

#include <shader.h>
#include <texture.h>
#include <animation.h>

typedef struct
{
    Animation currentAnimation;
    Shader *shader;
} SpriteRenderer;

SpriteRenderer *newSpriteRenderer(Shader *shader, Animation animation);
void freeSpriteRenderer(SpriteRenderer *renderer);

#endif