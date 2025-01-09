#include <object_pool_api.h>
#include <allocator.h>
#include <graphics_api.h>
#include <enemies/spawner.h>
#include <vfx/vfx.h>
#include <game.h>

ObjectPoolAPI *newObjectPoolAPI()
{
    ObjectPoolAPI *api = ALLOCATE(ObjectPoolAPI, 1);

    api->service.type = SERVICE_TYPE_OBJECT_POOL;
    api->service.init = initObjectPoolAPI;
    api->service.update = updateObjectPoolAPI;
    api->service.shutdown = shutdownObjectPoolAPI;

    return api;
}

void initObjectPoolAPI(void *self)
{
}

void updateObjectPoolAPI(void *self, float dt)
{
    CAST_API(ObjectPoolAPI, self);
    updateScene(api->scene, dt);
}

void shutdownObjectPoolAPI(void *self)
{
    CAST_API(ObjectPoolAPI, self);

    for (uint32_t i = 0; i < api->scene->entityCount; i++)
    {
        freeEntity(api->scene->entities[i]);
    }
}

void updateScene(Scene *scene, float dt)
{
    Entity **entities = scene->entities;
    GraphicsAPI *graphics = (GraphicsAPI *)getGameInstanceService(SERVICE_TYPE_GRAPHICS);
    Camera2D *camera = graphics->renderer->camera;

    for (uint32_t i = 0; i < scene->spawnerCount; i++)
    {
        updateSpawner(scene->spawners[i]);
    }

    for (uint32_t i = 0; i < scene->entityCount; i++)
    {
        if (isEntityOnScreen(entities[i], camera))
        {
            updateEntity(entities[i], dt);
        }
        else
        {
            if (!(entities[i]->type & (ENTITY_TYPE_COLLIDER | ENTITY_TYPE_VFX)))
            {
                destroyEntity(scene, entities[i]->index);
            }

            continue;
        }

        if ((entities[i]->type & ENTITY_TYPE_VFX))
        {
            Vfx *vfx = (Vfx *)entities[i];

            if (vfx->lifeTime <= 0.0f)
            {
                destroyEntity(scene, entities[i]->index);
            }
            else
            {
                vfx->lifeTime -= dt;
            }
            continue;
        }

        for (uint32_t j = 0; j < scene->entityCount; j++)
        {
            if (!isEntityOnScreen(entities[i], camera) || !isEntityOnScreen(entities[j], camera))
            {
                continue;
            }

            if (i == j ||
                ((entities[i]->type & ENTITY_TYPE_BRICK) && (entities[j]->type & ENTITY_TYPE_BRICK)) ||
                (entities[i]->type & ENTITY_TYPE_BRICK))
            {
                continue;
            }

            if (!(entities[i]->enableCollisions && entities[j]->enableCollisions))
            {
                continue;
            }

            if (AABBIntersect(entities[i], entities[j]))
            {
                AABBColisionData data = calculateCollisionData(entities[i], entities[j]);
                entities[i]->onCollision(entities[i], data);
            }
        }
    }

    for (uint32_t i = 0; i < scene->hudCount; i++)
    {
        scene->hudElements[i]->onUpdate(scene->hudElements[i], dt);
    }
}

void destroyEntity(Scene *scene, int index)
{
    Entity *entity = scene->entities[index];
    scene->entities[index] = scene->entities[scene->entityCount - 1];
    scene->entities[index]->index = index;
    entity->isEnabled = false;

    freeEntity(entity);
    scene->entityCount--;
}