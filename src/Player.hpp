#ifndef PLAYER_HPP
#define PLAYER_HPP
#include <SFML/Graphics.hpp>
#include "InputHandler.hpp"
#include "Socket.hpp"

class Player {
    public:
        Player();

        void update(InputHandler&, Socket&, float deltaTime) noexcept;
        void draw(sf::RenderWindow&) noexcept;

    private:
        sf::CircleShape m_sprite;
        sf::Vector2f m_velocity;
        sf::Vector2f m_previousVelocity;
        const float m_speed = 250.f;
        const float m_pi = 3.14159f;
};

#endif
