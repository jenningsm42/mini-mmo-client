#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <SFML/Graphics.hpp>

#include "GameObject.hpp"
#include "Character.hpp"

class Player : public GameObject {
public:
    Player(const Character&);

    virtual void update(Game&, const GameObjectCollection&, float deltaTime) noexcept override;

private:
    const float m_radius = 30.f;
    const float m_speed = 250.f;
    const float m_pi = 3.14159f;
    sf::CircleShape m_sprite;
    sf::Vector2f m_velocity;
    sf::Vector2f m_previousVelocity;

    virtual void draw(sf::RenderTarget&, sf::RenderStates) const override;
};

#endif // PLAYER_HPP
