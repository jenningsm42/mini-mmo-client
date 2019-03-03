#ifndef INPUTHANDLER_HPP
#define INPUTHANDLER_HPP

#include <SFML/Graphics.hpp>

class InputHandler {
public:
    InputHandler() noexcept;

    InputHandler(const InputHandler&);

    /* processEvent records any new incoming events */
    void processEvent(const sf::Event&) noexcept;

    /* get___Down returns true if the given key or
     * mouse button is being held down */
    bool getKeyDown(sf::Keyboard::Key) const noexcept;

    bool getMouseDown(sf::Mouse::Button) const noexcept;

    /* get___Tapped returns true if the given key or
     * mouse button has just been pressed, and not
     * necessarily held down */
    bool getKeyTapped(sf::Keyboard::Key) const noexcept;

    bool getMouseTapped(sf::Mouse::Button) const noexcept;

    /* getMousePosition returns the mouse position in
     * game screen coordinates */
    sf::Vector2i getMousePosition() const noexcept;

    /* update keeps track of whether a mouse button or key
     * has been tapped instead of continually held down.
     * It should be called once per frame to compare against
     * the previous button and key states.*/
    void update() noexcept;

private:
    bool m_keyDown[sf::Keyboard::KeyCount];
    bool m_previousKeyDown[sf::Keyboard::KeyCount];
    bool m_mouseDown[sf::Mouse::ButtonCount];
    bool m_previousMouseDown[sf::Mouse::ButtonCount];
    int m_mouseX, m_mouseY;
};

#endif // INPUTHANDLER_HPP
