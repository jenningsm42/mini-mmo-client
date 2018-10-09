#ifndef TEXTFIELD_HPP
#define TEXTFIELD_HPP
#include <SFML/Graphics.hpp>

#include "InputHandler.hpp"

class TextField {
    public:
        TextField(float width);

        void setActive(bool active, InputHandler* input = nullptr) noexcept;

        void update(InputHandler&, float deltaTime) noexcept;
        void draw(sf::RenderWindow&) noexcept;

        void setPosition(float x, float y) noexcept;
        sf::FloatRect getBoundingBox() const noexcept;

        std::string getInput() const noexcept;
        void clearInput() noexcept;

    private:
        bool m_active;
        float m_cursorTimer;
        std::string m_input;

        sf::Transform m_transform;
        sf::FloatRect m_boundingBox;
        sf::Font m_font;
        sf::Text m_inputText;
        sf::RectangleShape m_textBox;
        sf::RectangleShape m_textCursor;

        const float m_cursorShowTime = 1.f; // In seconds
};

#endif
