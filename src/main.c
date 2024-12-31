#include <game.h>

const int WIDTH = 800;
const int HEIGHT = 600;

int main(void)
{
    Game game;
    initGame(&game);
    setGameInstance(&game);
    gameLoop(&game);
    freeGame(&game);

    return 0;
}