#include <hud/lifebar.h>
#include <allocator.h>
#include "lifebar.h"
#include <graphics_api.h>
#include <game.h>

LifeBar *newLifebar(vec2s position)
{
    LifeBar *bar = ALLOCATE(LifeBar, 1);

    static bool animationLoaded = false;
    GraphicsAPI *graphics = (GraphicsAPI *)getGameInstanceService(SERVICE_TYPE_GRAPHICS);
    Shader *shader = getShader(graphics, SHADER_TYPE_SPRITE);
    Texture *texture = newTextureFromImage("../assets/sprites/hud/lifebar.png");
    SpriteRenderer *renderer = newSpriteRenderer(shader, newAnimation(1, 1, texture, false, PLAY_FROM_BEGIN));

    Entity entity;
    vec2s size = (vec2s){8.0f, 56.0f};

    initEntity(&entity, ENTITY_TYPE_HUD_ELEMENT, onUpdateLifeBar, onCollisionLifeBar, position, size, (vec2s){0.0f, 0.0f}, (vec2s){1.0f, 1.0f}, false, true, renderer);
    entity.isTiled = true;
    bar->entity = entity;
    bar->spawnPosition = position;
    bar->spawnSize = size;
    bar->value = 28;
    return bar;
}

LifeBarBG *newLifebarBG(vec2s position)
{
    LifeBarBG *bg = ALLOCATE(LifeBarBG, 1);

    static bool animationLoaded = false;
    GraphicsAPI *graphics = (GraphicsAPI *)getGameInstanceService(SERVICE_TYPE_GRAPHICS);
    Shader *shader = getShader(graphics, SHADER_TYPE_SPRITE);
    Texture *texture = newTextureFromImage("../assets/sprites/hud/lifebarbg.png");
    SpriteRenderer *renderer = newSpriteRenderer(shader, newAnimation(1, 1, texture, false, PLAY_FROM_BEGIN));

    Entity entity;

    initEntity(&entity, ENTITY_TYPE_HUD_ELEMENT, onUpdateNull, onCollisionNull, position, (vec2s){8.0f, 56.0f}, (vec2s){0.0f, 0.0f}, (vec2s){1.0f, 1.0f}, false, true, renderer);
    bg->entity = entity;
    return bg;
}

void onUpdateLifeBar(void *self, float dt)
{
    LifeBar *bar = (LifeBar *)self;

    float halfSpawnHeight = bar->spawnSize.y / 2.0f;
    float halfOriginalHeight = bar->entity.transform.scale.y / 2.0f;

    bar->entity.transform.scale.y = bar->value * 2.0f;
    bar->entity.transform.position.y = bar->spawnPosition.y + (halfSpawnHeight - halfOriginalHeight);
}

void onCollisionLifeBar(void *self, AABBColisionData data)
{
}
