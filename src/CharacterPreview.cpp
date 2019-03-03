#include "CharacterPreview.hpp"

CharacterPreview::CharacterPreview() {
    m_shape.setRadius(m_radius);
    m_shape.setPosition(285.f, 300.f);
    m_shape.setFillColor(sf::Color::White);
}

void CharacterPreview::setColor(const sf::Color& color) noexcept {
    m_shape.setFillColor(color);
}

const sf::Color& CharacterPreview::getColor() noexcept {
    return m_shape.getFillColor();
}

void CharacterPreview::update(Game&, const GameObjectCollection&, float) noexcept {
}

void CharacterPreview::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    target.draw(m_shape, states);
}
