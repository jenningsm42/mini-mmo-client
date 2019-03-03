#ifndef OTHERPLAYER_HPP
#define OTHERPLAYER_HPP

#include <SFML/Graphics.hpp>

#include "GameObject.hpp"

class OtherPlayer : public GameObject {
public:
    OtherPlayer(
        float x,
        float y,
        float velocityX,
        float velocityY,
        const sf::Color&,
        const std::string& name);

    virtual void update(Game&, const GameObjectCollection&, float deltaTime) noexcept override;

    void setVelocity(float x, float y, float velocityX, float velocityY) noexcept;

    operator std::string() const noexcept;

private:
    sf::CircleShape m_sprite;
    sf::Vector2f m_velocity;
    std::string m_name;

    virtual void draw(sf::RenderTarget&, sf::RenderStates) const override;
};

#endif // OTHERPLAYER_HPP
