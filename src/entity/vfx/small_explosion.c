#include <vfx/small_explosion.h>
#include <game.h>
#include <graphics_api.h>
#include <audio_api.h>
#include <allocator.h>

SmallExplosion *newSmallExplosion(vec2s position)
{
    static bool animationCreated = false;
    GraphicsAPI *graphics = (GraphicsAPI *)getGameInstanceService(SERVICE_TYPE_GRAPHICS);
    AudioAPI *audio = (AudioAPI *)getGameInstanceService(SERVICE_TYPE_AUDIO);
    Shader *shader = getShader(graphics, SHADER_TYPE_SPRITE);
    Texture *texture = newTextureFromImage("../assets/sprites/vfx/small_explosion.png");
    SpriteRenderer *renderer = newSpriteRenderer(shader, newAnimation(5, 20, texture, false, PLAY_FROM_BEGIN));
    SmallExplosion *explosion = ALLOCATE(SmallExplosion, 1);

    Entity entity;
    Vfx vfx;

    initEntity(&entity, ENTITY_TYPE_VFX, onUpdateNull, onCollisionNull, position, (vec2s){32.0f, 32.0f}, (vec2s){0.0f, 0.0f}, (vec2s){32.0f, 32.0f}, false, true, renderer);
    entity.enableCollisions = false;

    initVfx(&vfx, entity, VFX_SMALL_EXPLOSION, 0.25f);
    explosion->vfx = vfx;

    playAudioWAV(audio, "../assets/audio/sfx/Explosion.wav", AUDIO_SMALL_EXPLOSION);

    return explosion;
}