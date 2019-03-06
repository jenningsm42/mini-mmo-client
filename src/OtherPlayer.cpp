#include "OtherPlayer.hpp"

OtherPlayer::OtherPlayer(
    float x,
    float y,
    float velocityX,
    float velocityY,
    const sf::Color& color,
    const std::string& name
) {
    m_sprite.setRadius(30.f);
    m_sprite.setFillColor(color);
    m_sprite.setPosition(x, y);
    m_velocity = sf::Vector2f(velocityX, velocityY);
    m_name = name;
}

void OtherPlayer::update(Game&, const GameObjectCollection&, float deltaTime) noexcept {
    m_sprite.move(m_velocity.x * deltaTime, m_velocity.y * deltaTime);
}

void OtherPlayer::setVelocity(float x, float y, float velocityX, float velocityY) noexcept {
    m_sprite.setPosition(x, y);
    m_velocity = sf::Vector2f(velocityX, velocityY);
}

std::string OtherPlayer::getName() const noexcept {
    return m_name;
}

void OtherPlayer::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    target.draw(m_sprite, states);
}

OtherPlayer::operator std::string() const noexcept {
    auto position = m_sprite.getPosition();
    return "<\"" + m_name + "\" (" +
        std::to_string(position.x) + ", " +
        std::to_string(position.y) + ")>";
}
