#ifndef OTHERPLAYER_HPP
#define OTHERPLAYER_HPP
#include <SFML/Graphics.hpp>

class OtherPlayer {
    public:
        OtherPlayer(float x, float y, float velocityX, float velocityY,
            const sf::Color&, const std::string& name);

        void update(float deltaTime) noexcept;
        void draw(sf::RenderWindow&) noexcept;

        void setVelocity(float x, float y, float velocityX, float velocityY) noexcept;

    private:
        sf::CircleShape m_sprite;
        sf::Vector2f m_velocity;
        std::string m_name;
};

#endif
