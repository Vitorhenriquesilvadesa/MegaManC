#include <object_pool_api.h>
#include <allocator.h>

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
}

void updateScene(Scene *scene, float dt)
{
    for (uint32_t i = 0; i < scene->entityCount; i++)
    {
        updateEntity(scene->entities[i], dt);
    }
}