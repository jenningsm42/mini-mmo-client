#ifndef INPUTHANDLER_HPP
#define INPUTHANDLER_HPP
#include <queue>
#include <SFML/Graphics.hpp>

class InputHandler {
    public:
        InputHandler() noexcept;

        void processEvent(const sf::Event&) noexcept;

        bool getKeyDown(sf::Keyboard::Key) const noexcept;
        bool getMouseDown(sf::Mouse::Button) const noexcept;

        bool getMouseClicked(sf::Mouse::Button) const noexcept;
        sf::Vector2i getMousePosition() const noexcept;

        // Calling this will empty the current queue of symbols entered
        std::queue<uint32_t> getTextEntered() noexcept;

        // Must be called once per frame for mouse clicks
        void update() noexcept;

    private:
        bool m_keyDown[sf::Keyboard::KeyCount];
        bool m_mouseDown[sf::Mouse::ButtonCount];
        bool m_previousMouseDown[sf::Mouse::ButtonCount];
        int m_mouseX, m_mouseY;
        std::queue<uint32_t> m_textEntered;
        const unsigned int m_maxTextEnteredSize = 50;
};

#endif
