#include <game.h>
#include <iterator.h>
#include <allocator.h>
#include <graphics_api.h>
#include <object_pool_api.h>
#include <trigger.h>
#include <stdio.h>
#include <animation.h>
#include <test_scene.h>

#include <GLFW/glfw3.h>

static Game *instance = NULL;

Service *getGameInstanceService(ServiceType type)
{
    return instance->services.services[type];
}

void gameLoop(Game *game)
{
    updateGame(game, true);

    float accumulatedTime = 0.0f;
    float lastTime = (float)glfwGetTime();
    game->lt = lastTime;

    while (!getGameInstanceFlag(FLAG_WINDOW_CLOSED, SERVICE_TYPE_GRAPHICS))
    {
        float currentTime = (float)glfwGetTime();
        float deltaTime = currentTime - lastTime;
        lastTime = currentTime;

        if (deltaTime > game->maxDeltaTime)
        {
            deltaTime = game->maxDeltaTime;
        }

        accumulatedTime += deltaTime;

        if (game->lockFps)
        {
            bool canRender = accumulatedTime >= 1.0f / game->minFps;

            if (canRender)
            {
                accumulatedTime = 0.0f;
            }
            updateGame(game, canRender);
        }
        else
        {
            updateGame(game, true);
        }
    }
}

void calculateGameDeltaTime(Game *game)
{
    float currentTime = (float)glfwGetTime();
    float deltaTime = currentTime - game->lt;
    game->lt = currentTime;
    game->enableUpdates = true;
    game->dt = deltaTime;

    if (game->dt > game->maxDeltaTime)
    {
        game->dt = game->maxDeltaTime;
    }
}

void initGame(Game *game, float minFps, float maxDeltaTime, bool lockFps)
{
    game->flags = ALLOCATE(FlagFunction, FLAG_MAX_COUNT);
    initServiceSet(&game->services);
    registerServices(game);

    game->isRunning = true;
    game->minFps = minFps;
    game->maxDeltaTime = maxDeltaTime;
    game->lockFps = lockFps;
    game->fixedDt = 1.0f / minFps;

    initServices(game);

    Scene *scene = getTestScene();

    CAST_API(ObjectPoolAPI, game->services.services[SERVICE_TYPE_OBJECT_POOL]);
    api->scene = scene;
}

void updateGame(Game *game, bool render)
{
    if (game->enableUpdates)
    {
        calculateGameDeltaTime(game);

        if (!game->lockFps)
        {
            while (game->dt >= game->fixedDt)
            {
                for (size_t i = 0; i < SERVICE_TYPE_MAX; i++)
                {
                    Service *service = game->services.services[i];
                    if (service->type != SERVICE_TYPE_GRAPHICS)
                    {
                        service->update(service, game->fixedDt);
                    }
                }
                game->dt -= game->fixedDt;
            }
        }

        for (size_t i = 0; i < SERVICE_TYPE_MAX; i++)
        {
            Service *service = game->services.services[i];
            if (service->type == SERVICE_TYPE_GRAPHICS && !render)
            {
                continue;
            }
            service->update(service, game->dt);
        }
    }

    glfwPollEvents();
}

void freeGame(Game *game)
{
    for (size_t i = 0; i < SERVICE_TYPE_MAX; i++)
    {
        Service *service = game->services.services[i];
        service->shutdown(service);
    }

    FREE(game->services.services);
}

void registerService(Game *game, Service *service)
{
    setServiceFromArray(&game->services, service);
}

void registerServices(Game *game)
{
    registerGraphicsAPI(game);
    registerObjectPoolAPI(game);
    registerEventAPI(game);
}

void registerGraphicsAPI(Game *game)
{
    vec4s backgroundColor = {0.0f, 0.0f, 0.0f, 1.0f};
    vec2s windowSize = {256 * 4, 224 * 4};
    GraphicsAPI *graphics = newGraphicsAPI(windowSize, "Mega Man", backgroundColor);
    registerService(game, AS_SERVICE_PTR(graphics));
    game->flags[FLAG_WINDOW_CLOSED] = graphics->isWindowClosed;
}

void registerObjectPoolAPI(Game *game)
{
    ObjectPoolAPI *pool = newObjectPoolAPI();
    registerService(game, AS_SERVICE_PTR(pool));
}

void registerEventAPI(Game *game)
{
    TriggerAPI *triggers = newTriggerAPI();
    registerService(game, AS_SERVICE_PTR(triggers));
}

void initServices(Game *game)
{
    for (size_t i = 0; i < SERVICE_TYPE_MAX; i++)
    {
        Service *service = game->services.services[i];
        service->init(service);
    }
}

void setGameInstance(Game *game)
{
    instance = game;
}

void setGameInstanceFlag(GameFlag flag, FlagFunction function)
{
    instance->flags[flag] = function;
}

bool getGameInstanceFlag(GameFlag flag, ServiceType type)
{
    return instance->flags[flag]((void *)instance->services.services[type]);
}

Scene *getGameInstanceActiveScene()
{
    CAST_API(ObjectPoolAPI, instance->services.services[SERVICE_TYPE_OBJECT_POOL]);

    return api->scene;
}

float getGameInstanceDeltaTime()
{
    return instance->dt;
}

float getGameInstanceFixedDeltaTime()
{
    return instance->fixedDt;
}

float getGameInstanceFPS()
{
    return instance->minFps;
}

void setGameEnableUpdates(bool enable)
{
    instance->enableUpdates = enable;
}

bool isKeyPressed(uint32_t key)
{
    GraphicsAPI *graphics = (GraphicsAPI *)getGameInstanceService(SERVICE_TYPE_GRAPHICS);
    GLFWwindow *window = graphics->window->nativeWindow;
    return glfwGetKey(window, key) == GLFW_PRESS;
}