#include <game.h>

const int WIDTH = 800;
const int HEIGHT = 600;

int main(void)
{
    Game game;
    setGameInstance(&game);
    initGame(&game);
    gameLoop(&game);
    freeGame(&game);

    return 0;
}