#include <enemies/metal/metal.h>
#include <megaman/weapons/ally_weapon.h>
#include <megaman/weapons/mega_buster.h>
#include <megaman/megaman.h>
#include <vfx/small_explosion.h>
#include <game.h>
#include <graphics_api.h>
#include <object_pool_api.h>
#include <trigger.h>
#include <audio_api.h>
#include <aabb.h>
#include <string.h>
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
    initEnemy(&enemy, entity, ENEMY_TYPE_METAL, METAL_MAX_HP);

    metal->attackTimer = 0.0f;
    metal->defendTimer = METAL_DEFENT_TIMER_MAX;

    metal->enemy = enemy;
    metal->shooted = false;
    metal->state = METAL_STATE_DEFEND;

    TriggerAPI *triggers = (TriggerAPI *)getGameInstanceService(SERVICE_TYPE_EVENT);
    Id id = metal->enemy.entity.id;

    registerTrigger(triggers, id, onCollisionWithMegamanTrigger, onCollisionWithMegaman, metal);
    registerTrigger(triggers, id, onMetalAttackTrigger, onMetalAttack, metal);

    return metal;
}

MetalShoot *newMetalShoot(vec2s position, float angle)
{
    static GraphicsAPI *graphics = NULL;
    static Shader *shader = NULL;
    static Texture *texture = NULL;

    static bool isAnimationCreated = false;

    if (!isAnimationCreated)
    {
        isAnimationCreated = true;
        graphics = (GraphicsAPI *)getGameInstanceService(SERVICE_TYPE_GRAPHICS);
        shader = getShader(graphics, SHADER_TYPE_SPRITE);
        texture = newTextureFromImage("../assets/sprites/enemies/metal/shoot.png");
    }

    Animation animation = newAnimation(1, 1, texture, false, PLAY_FROM_BEGIN);

    SpriteRenderer *renderer = newSpriteRenderer(shader, animation);
    Entity entity;

    initEntity(&entity, ENTITY_TYPE_ENEMY, onUpdateMetalShoot, onCollisionMetalShoot, position, (vec2s){8, 8},
               (vec2s){0.0f, 0.0f}, (vec2s){6.0f, 6.0f}, false, true, renderer);

    MetalShoot *shoot = ALLOCATE(MetalShoot, 1);
    memset(shoot, 0, sizeof(MetalShoot));

    Enemy enemy;
    initEnemy(&enemy, entity, ENEMY_TYPE_METAL, 1);

    shoot->enemy = enemy;
    shoot->angle = angle;

    AudioAPI *audio = (AudioAPI *)getGameInstanceService(SERVICE_TYPE_AUDIO);
    playAudioWAV(audio, "../assets/audio/sfx/EnemyShoot.wav", AUDIO_ENEMY_SHOOT);

    return shoot;
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

    if (metal->attackTimer < 0.5f && !metal->shooted && metal->state == METAL_STATE_ATTACK)
    {
        metal->shooted = true;
        Scene *scene = getGameInstanceActiveScene();
        float angle = metal->enemy.entity.isMirrored ? 0 : 180;
        vec2s position = (vec2s){metal->enemy.entity.transform.position.x, metal->enemy.entity.transform.position.y + 3.0f};
        addObjectToScene(scene, AS_ENTITY_PTR(newMetalShoot(position, angle)));
    }

    if (metal->attackTimer > 0.0f)
    {
        metal->attackTimer -= dt;
    }
    else
    {
        metal->shooted = false;
        setAnimation(AS_ENTITY_PTR(metal), metalResources.animations[METAL_STATE_DEFEND], PLAY_FROM_BEGIN, false);
        metal->state = METAL_STATE_DEFEND;
    }

    if (metal->state == METAL_STATE_DEFEND)
    {
        metal->enemy.entity.isMirrored = metal->enemy.entity.transform.position.x < megaman->entity.transform.position.x;
    }
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

                metal->defendTimer = METAL_DEFENT_TIMER_MAX / 3.0f;

                if (shoot->weapon.entity.transform.position.x < metal->enemy.entity.transform.position.x)
                {
                    shoot->angle = -45.0f * 3.0f;
                }
                else
                {
                    shoot->angle = -45.0f;
                }
                break;
            }

            case METAL_STATE_ATTACK:
            {
                metal->enemy.life -= weapon->damage;
                shoot->weapon.entity.enableCollisions = false;
                Scene *scene = getGameInstanceActiveScene();
                vec2s position = metal->enemy.entity.transform.position;

                destroyEntity(scene, shoot->weapon.entity.index);

                if (metal->enemy.life == 0)
                {
                    destroyEntity(scene, metal->enemy.entity.index);
                    addObjectToScene(scene, AS_ENTITY_PTR(newSmallExplosion(position)));
                }
                break;
            }
            }
        }
    }
}

void onUpdateMetalShoot(void *self, float dt)
{
    MetalShoot *shoot = (MetalShoot *)self;

    float angleRadians = glm_rad(shoot->angle);

    float dx = cos(angleRadians) * 120.0f * dt;
    float dy = sin(angleRadians) * 120.0f * dt;

    shoot->enemy.entity.transform.position.x += dx;
    shoot->enemy.entity.transform.position.y += dy;
}

void onCollisionMetalShoot(void *self, AABBColisionData data)
{
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

    return metal->defendTimer <= 0.0f;
}

void onMetalAttack(void *self)
{
    Metal *metal = (Metal *)self;
    metal->attackTimer = 1.0f;
    metal->state = METAL_STATE_ATTACK;
    setAnimation(AS_ENTITY_PTR(metal), metalResources.animations[METAL_STATE_ATTACK], PLAY_FROM_BEGIN, false);
}