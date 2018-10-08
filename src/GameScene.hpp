#ifndef GAMESCENE_HPP
#define GAMESCENE_HPP
#include "Scene.hpp"
#include "Player.hpp"
#include "PlayerPool.hpp"

class GameScene : public Scene {
    public:
        GameScene(Socket&);

        void processMessages(std::queue<Message>&, Socket&) override;
        void update(InputHandler&, Socket&, float deltaTime) noexcept override;
        void draw(sf::RenderWindow&) noexcept override;

    private:
        Player m_player;
        PlayerPool m_playerPool;
};

#endif
