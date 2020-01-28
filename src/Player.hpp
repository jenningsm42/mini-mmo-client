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
    static constexpr float Speed = 250.f;
    static constexpr float AABBWidth = 50.f;
    static constexpr float AABBHeight = 40.f;
    bool m_controlsEnabled;
    sf::Vector2f m_velocity;
    sf::Vector2f m_previousVelocity;
};

#endif // PLAYER_HPP
