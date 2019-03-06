#include "Character.hpp"

Character::Character(uint32_t id, const std::string& name, float x, float y, const sf::Color& color)
    : m_id(id), m_name(name), m_position(x, y), m_color(color) {
}

Character::Character(const Character& other) {
    m_id = other.m_id;
    m_name = other.m_name;
    m_position = sf::Vector2f(other.m_position);
    m_color = sf::Color(other.m_color);
}

uint32_t Character::getId() const noexcept {
    return m_id;
}

std::string Character::getName() const noexcept {
    return m_name;
}

sf::Vector2f Character::getPosition() const noexcept {
    return m_position;
}

sf::Color Character::getColor() const noexcept {
    return m_color;
}
