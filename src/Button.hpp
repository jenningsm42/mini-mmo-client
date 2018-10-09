#ifndef BUTTON_HPP
#define BUTTON_HPP
#include <SFML/Graphics.hpp>

#include "InputHandler.hpp"

class Button {
    public:
        Button(float width, const std::string& name);
        Button(const Button&);

        void draw(sf::RenderWindow&) noexcept;

        void setPosition(float x, float y) noexcept;

        bool getClicked(InputHandler&) noexcept;

    private:
        sf::Transform m_transform;
        sf::FloatRect m_boundingBox;
        sf::Font m_font;
        sf::Text m_buttonText;
        sf::RectangleShape m_textBox;
};

#endif
