#include <sprite.h>
#include <allocator.h>
#include <entity.h>

SpriteRenderer *newSpriteRenderer(Shader *shader, Animation *animation)
{
    SpriteRenderer *renderer = ALLOCATE(SpriteRenderer, 1);
    renderer->shader = shader;
    renderer->currentAnimation = animation;

    return renderer;
}