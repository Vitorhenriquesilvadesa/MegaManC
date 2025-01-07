#include <megaman/weapons/mega_buster.h>
#include <game.h>
#include <graphics_api.h>
#include <allocator.h>
#include <enemy.h>
#include <enemies/metal/metal.h>
#include <audio_api.h>
#include <entity.h>
#include <object_pool_api.h>

#define ALLY_NORMAL_SHOOT_SPEED 400

MegaBusterShoot *newMegaBusterShoot(vec2s position, float angle)
{
    GraphicsAPI *graphics = (GraphicsAPI *)getGameInstanceService(SERVICE_TYPE_GRAPHICS);
    Shader *shader = getShader(graphics, SHADER_TYPE_SPRITE);
    Texture *texture = newTextureFromImage("../assets/sprites/megaman/weapons/normal/1.png");
    Animation animation = newAnimation(1, 1, texture, false, PLAY_FROM_BEGIN);

    SpriteRenderer *renderer = newSpriteRenderer(shader, animation);
    Entity entity;

    initEntity(&entity, ENTITY_TYPE_ALLY_SHOOT, onUpdateMegaBusterShoot, onCollisionMegaBusterShoot, position, (vec2s){8, 6},
               (vec2s){0.0f, 0.0f}, (vec2s){8.0f, 6.0f}, false, true, renderer);

    MegaBusterShoot *shoot = ALLOCATE(MegaBusterShoot, 1);
    AllyWeapon weapon;
    initAllyWeapon(&weapon, entity, ALLY_WEAPON_MEGA_BUSTER, 1);

    shoot->weapon = weapon;
    shoot->angle = angle;
    shoot->speed = ALLY_NORMAL_SHOOT_SPEED;

    AudioAPI *audio = (AudioAPI *)getGameInstanceService(SERVICE_TYPE_AUDIO);
    playAudioWAV(audio, "../assets/audio/sfx/MegaBuster.wav", AUDIO_MEGA_BUSTER);

    return shoot;
}

void onUpdateMegaBusterShoot(void *self, float dt)
{
    MegaBusterShoot *shoot = (MegaBusterShoot *)self;
    float angleRadians = glm_rad(shoot->angle);

    float dx = cos(angleRadians) * shoot->speed * dt;
    float dy = sin(angleRadians) * shoot->speed * dt;

    shoot->weapon.entity.transform.position.x += dx;
    shoot->weapon.entity.transform.position.y += dy;
}

void onCollisionMegaBusterShoot(void *self, AABBColisionData data)
{
    MegaBusterShoot *shoot = (MegaBusterShoot *)self;
}
