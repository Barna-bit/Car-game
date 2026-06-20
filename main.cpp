#include "Game.hpp"
#include <ctime>
#include <cstdlib>

int main() {
    std::srand(static_cast<unsigned int>(std::time(NULL)));
    Game game;
    game.run();
    return 0;
}
