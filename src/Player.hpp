#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <SFML/Graphics.hpp>

#include "Character.hpp"

class Player : public Character {
public:
    Player(Game&, const Character&);
    Player(const Player&);
    Player(Player&&);

    virtual void update(Game&, GameObjectCollection&, float deltaTime) noexcept override;

    void setControlsEnabled(bool enabled) noexcept;

private:
    const float m_radius = 30.f;
    const float m_speed = 250.f;
    const float m_pi = 3.14159f;
    bool m_controlsEnabled;
    sf::Vector2f m_velocity;
    sf::Vector2f m_previousVelocity;
};

#endif // PLAYER_HPP
