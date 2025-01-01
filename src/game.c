#include <game.h>
#include <iterator.h>
#include <allocator.h>
#include <graphics_api.h>
#include <object_pool_api.h>
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
    Scene *scene = getTestScene();
    CAST_API(ObjectPoolAPI, game->services.services[SERVICE_TYPE_OBJECT_POOL]);
    api->scene = scene;

    while (!getGameInstanceFlag(FLAG_WINDOW_CLOSED, SERVICE_TYPE_GRAPHICS))
    {
        updateGame(game);
    }
}

void calculateGameDeltaTime(Game *game)
{
    static float lastTime = 0.0f;
    float currentTime = (float)glfwGetTime();
    game->dt = currentTime - lastTime;
    lastTime = currentTime;
}

void initGame(Game *game)
{
    game->flags = ALLOCATE(FlagFunction, FLAG_MAX_COUNT);
    initServiceSet(&game->services);
    registerServices(game);

    game->isRunning = true;

    initServices(game);

    Scene *scene = newScene();

    CAST_API(ObjectPoolAPI, game->services.services[SERVICE_TYPE_OBJECT_POOL]);
    api->scene = scene;
}

void updateGame(Game *game)
{
    calculateGameDeltaTime(game);

    for (size_t i = 0; i < SERVICE_TYPE_MAX; i++)
    {
        Service *service = game->services.services[i];
        service->update(service, game->dt);
    }
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

bool isKeyPressed(uint32_t key)
{
    GraphicsAPI *graphics = (GraphicsAPI *)getGameInstanceService(SERVICE_TYPE_GRAPHICS);
    GLFWwindow *window = graphics->window->nativeWindow;
    return glfwGetKey(window, key) == GLFW_PRESS;
}