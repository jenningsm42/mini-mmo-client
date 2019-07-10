#ifndef OTHERPLAYER_HPP
#define OTHERPLAYER_HPP

#include <SFML/Graphics.hpp>

#include "Character.hpp"

class OtherPlayer : public Character {
public:
    OtherPlayer(Game&, const Character&);
    OtherPlayer(const OtherPlayer&);
    OtherPlayer(OtherPlayer&&);

    virtual void update(Game&, const GameObjectCollection&, float deltaTime) noexcept override;

    void setVelocity(const sf::Vector2f& position, const sf::Vector2f& velocity) noexcept;

    operator std::string() const noexcept;

private:
    sf::Vector2f m_velocity;
};

#endif // OTHERPLAYER_HPP
