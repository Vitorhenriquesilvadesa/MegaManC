#include <game.h>

int main(void)
{
    Game game;
    setGameInstance(&game);
    initGame(&game, 240.0f, 1.0f / 60.0f, true);
    gameLoop(&game);
    freeGame(&game);

    return 0;
}