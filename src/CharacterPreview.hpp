#ifndef CHARACTERPREVIEW_HPP
#define CHARACTERPREVIEW_HPP

#include <SFML/Graphics.hpp>

#include "GameObject.hpp"

class CharacterPreview : public GameObject {
public:
    CharacterPreview();

    void setColor(const sf::Color&) noexcept;

    const sf::Color& getColor() noexcept;

    virtual void update(Game&, const GameObjectCollection&, float deltaTime) noexcept override;

private:
    const float m_radius = 30.f;
    sf::CircleShape m_shape;

    virtual void draw(sf::RenderTarget&, sf::RenderStates) const override;
};

#endif // CHARACTERPREVIEW_HPP
