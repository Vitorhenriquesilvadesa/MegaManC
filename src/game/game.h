#ifndef GAME_H
#define GAME_H

#include <service.h>
#include <types.h>
#include <flags.h>

typedef struct
{
    ServiceSet services;
    bool isRunning;
    FlagFunction *flags;
} Game;

void gameLoop(Game *game);
void initGame(Game *game);
void updateGame(Game *game);
void freeGame(Game *game);
void registerService(Game *game, Service *service);
void registerServices(Game *game);
void registerGraphicsAPI(Game *game);
void initServices(Game *game);
void setGameInstance(Game *game);
void setGameInstanceFlag(GameFlag flag, FlagFunction function);
bool getGameInstanceFlag(GameFlag flag, ServiceType type);

#endif