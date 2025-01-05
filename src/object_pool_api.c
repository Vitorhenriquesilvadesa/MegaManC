#include <object_pool_api.h>
#include <allocator.h>
#include <graphics_api.h>
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

    for (uint32_t i = 0; i < scene->entityCount; i++)
    {
        if (isEntityOnScreen(entities[i], camera))
        {
            updateEntity(entities[i], dt);
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

            if (AABBIntersect(entities[i], entities[j]))
            {
                AABBColisionData data = calculateCollisionData(entities[i], entities[j]);
                entities[i]->onCollision(entities[i], data);
            }
        }
    }
}
