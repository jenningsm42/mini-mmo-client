#include "Game.hpp"
#include "LoginScene.hpp"

int main() {
    Game game;
    game.setScene(std::make_shared<LoginScene>());
    game.run();
    return 0;
}
