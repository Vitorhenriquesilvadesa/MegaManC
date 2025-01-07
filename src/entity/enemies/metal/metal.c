#include <enemies/metal/metal.h>
#include <megaman/weapons/ally_weapon.h>
#include <megaman/weapons/mega_buster.h>
#include <megaman/megaman.h>
#include <game.h>
#include <graphics_api.h>
#include <trigger.h>
#include <audio_api.h>
#include <aabb.h>
#include <allocator.h>

#define METAL_DEFENT_TIMER_MAX 3.0f
#define METAL_DEFENT_TIMER_INCREMENT 0.1f

static MetalResources metalResources;

static void createAnimations()
{
    metalResources.animations = ALLOCATE(Animation, METAL_STATE_MAX);

    {
        Texture *texture = newTextureFromImage("../assets/sprites/enemies/metal/attack.png");
        metalResources.animations[METAL_STATE_ATTACK] = newAnimation(11, 11, texture, false, PLAY_FROM_BEGIN);
    }

    {
        Texture *texture = newTextureFromImage("../assets/sprites/enemies/metal/idle.png");
        metalResources.animations[METAL_STATE_DEFEND] = newAnimation(1, 1, texture, false, PLAY_FROM_BEGIN);
    }
}

Metal *newMetal(vec2s position)
{
    GraphicsAPI *graphics = (GraphicsAPI *)getGameInstanceService(SERVICE_TYPE_GRAPHICS);
    Shader *shader = getShader(graphics, SHADER_TYPE_SPRITE);

    static bool animationsCreated = false;

    if (!animationsCreated)
    {
        createAnimations();
        animationsCreated = true;
    }

    SpriteRenderer *renderer = newSpriteRenderer(shader, metalResources.animations[METAL_STATE_DEFEND]);

    Metal *metal = ALLOCATE(Metal, 1);
    Entity entity;
    initEntity(&entity, ENTITY_TYPE_ENEMY, onUpdateMetal, onCollisionMetal,
               position, (vec2s){32.0f, 32.0f}, (vec2s){0, 0}, (vec2s){14.0f, 15.0f},
               false, true, renderer);

    Enemy enemy;
    initEnemy(&enemy, entity, ENEMY_TYPE_METAL);

    metal->attackTimer = 0.0f;
    metal->defendTimer = METAL_DEFENT_TIMER_MAX;

    metal->enemy = enemy;
    metal->state = METAL_STATE_DEFEND;

    TriggerAPI *triggers = (TriggerAPI *)getGameInstanceService(SERVICE_TYPE_EVENT);
    registerTrigger(triggers, onCollisionWithMegamanTrigger, onCollisionWithMegaman, metal);
    registerTrigger(triggers, onMetalAttackTrigger, onMetalAttack, metal);

    return metal;
}

void onUpdateMetal(void *self, float dt)
{
    Scene *scene = getGameInstanceActiveScene();
    Megaman *megaman = (Megaman *)scene->metadata;
    Metal *metal = (Metal *)self;

    if (metal->defendTimer > 0.0f)
    {
        metal->defendTimer -= dt;
    }
    else if (metal->attackTimer <= 0.0f)
    {
        metal->defendTimer = METAL_DEFENT_TIMER_MAX;
    }

    if (metal->attackTimer > 0.0f)
    {
        metal->attackTimer -= dt;
    }
    else
    {
        setAnimation(AS_ENTITY_PTR(metal), metalResources.animations[METAL_STATE_DEFEND], PLAY_FROM_BEGIN, false);
        metal->state = METAL_STATE_DEFEND;
    }

    metal->enemy.entity.isMirrored = metal->enemy.entity.transform.position.x < megaman->entity.transform.position.x;
}

void onCollisionMetal(void *self, AABBColisionData data)
{
    Metal *metal = (Metal *)self;
    AudioAPI *audio = (AudioAPI *)getGameInstanceService(SERVICE_TYPE_AUDIO);

    if (data.other->type == ENTITY_TYPE_ALLY_SHOOT)
    {
        AllyWeapon *weapon = (AllyWeapon *)data.other;

        if (weapon->type == ALLY_WEAPON_MEGA_BUSTER)
        {
            MegaBusterShoot *shoot = (MegaBusterShoot *)weapon;

            switch (metal->state)
            {
            case METAL_STATE_DEFEND:
            {
                playAudioWAV(audio, "../assets/audio/sfx/Dink.wav", AUDIO_DINK);
                shoot->weapon.entity.enableCollisions = false;

                metal->defendTimer = METAL_DEFENT_TIMER_MAX;

                if (shoot->weapon.entity.transform.position.x < metal->enemy.entity.transform.position.x)
                {
                    shoot->angle = -45.0f * 3.0f;
                }
                else
                {
                    shoot->angle = -45.0f;
                }
            }

            case METAL_STATE_ATTACK:
            {
                metal->enemy.life -= weapon->damage;
            }
            }
        }
    }
}

bool onCollisionWithMegamanTrigger(void *self)
{
    Scene *scene = getGameInstanceActiveScene();
    Megaman *megaman = (Megaman *)scene->metadata;
    Metal *metal = (Metal *)self;
    return AABBIntersect(AS_ENTITY_PTR(megaman), AS_ENTITY_PTR(metal));
}

void onCollisionWithMegaman(void *self)
{
    printf("Colidiu com Megaman\n");
}

bool onMetalAttackTrigger(void *self)
{
    Metal *metal = (Metal *)self;
    metal->state = METAL_STATE_ATTACK;
    return metal->defendTimer <= 0.0f;
}

void onMetalAttack(void *self)
{
    Metal *metal = (Metal *)self;
    metal->attackTimer = 1.0f;
    setAnimation(AS_ENTITY_PTR(metal), metalResources.animations[METAL_STATE_ATTACK], PLAY_FROM_BEGIN, false);
}