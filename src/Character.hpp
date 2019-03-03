#ifndef CHARACTER_HPP
#define CHARACTER_HPP
#include <SFML/Graphics.hpp>

class Character {
public:
    Character(uint32_t id, float x, float y, const sf::Color&);
    Character(const Character&);

    uint32_t getId() const noexcept;
    sf::Vector2f getPosition() const noexcept;
    sf::Color getColor() const noexcept;

private:
    uint32_t m_id;
    sf::Vector2f m_position;
    sf::Color m_color;
};

#endif // CHARACTER_HPP
