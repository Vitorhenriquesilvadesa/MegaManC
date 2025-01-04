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
    float dt;
    float lt;
    FlagFunction *flags;
    Scene *activeScene;
} Game;

Service *getGameInstanceService(ServiceType type);
Scene *getGameInstanceActiveScene();
float getGameInstanceDeltaTime();
void gameLoop(Game *game);
void calculateGameDeltaTime(Game *game);
void initGame(Game *game);
void updateGame(Game *game);
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