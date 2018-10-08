#include "OtherPlayer.hpp"

OtherPlayer::OtherPlayer(float x, float y, float velocityX, float velocityY) {
    m_sprite.setRadius(30.f);
    m_sprite.setFillColor(sf::Color::Green);
    m_sprite.setPosition(x, y);
    m_velocity = sf::Vector2f(velocityX, velocityY);
}

void OtherPlayer::update(float deltaTime) noexcept {
    m_sprite.move(m_velocity.x * deltaTime, m_velocity.y * deltaTime);
}

void OtherPlayer::draw(sf::RenderWindow& window) noexcept {
    window.draw(m_sprite);
}

void OtherPlayer::setVelocity(float x, float y, float velocityX, float velocityY) noexcept {
    m_sprite.setPosition(x, y);
    m_velocity = sf::Vector2f(velocityX, velocityY);
}
