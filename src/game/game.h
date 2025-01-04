#ifndef GAME_H
#define GAME_H

#include <service.h>
#include <types.h>
#include <flags.h>
#include <renderer.h>

#define FROM_SERVICE(serviceType, type) (type *)getGameInstanceService(serviceType)

typedef struct
{
    ServiceSet services;
    bool isRunning;
    bool lockFps;
    bool enableUpdates;
    float maxDeltaTime;
    float minFps;
    float dt;
    float lt;
    float fixedDt;
    FlagFunction *flags;
    Scene *activeScene;
} Game;

Service *getGameInstanceService(ServiceType type);
Scene *getGameInstanceActiveScene();
float getGameInstanceDeltaTime();
float getGameInstanceFixedDeltaTime();
float getGameInstanceFPS();
void setGameEnableUpdates(bool enable);
void gameLoop(Game *game);
void calculateGameDeltaTime(Game *game);
void initGame(Game *game, float minFps, float maxDeltaTime, bool lockFps);
void updateGame(Game *game, bool render);
void freeGame(Game *game);
void registerService(Game *game, Service *service);
void registerServices(Game *game);
void registerGraphicsAPI(Game *game);
void registerObjectPoolAPI(Game *game);
void registerEventAPI(Game *game);
void initServices(Game *game);
void setGameInstance(Game *game);
void setGameInstanceFlag(GameFlag flag, FlagFunction function);
bool getGameInstanceFlag(GameFlag flag, ServiceType type);

bool isKeyPressed(uint32_t key);

#endif