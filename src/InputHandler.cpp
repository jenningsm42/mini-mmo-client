#include "InputHandler.hpp"

InputHandler::InputHandler() noexcept {
    for (auto& key : m_keyDown) {
        key = false;
    }
    for (auto& button : m_mouseDown) {
        button = false;
    }
    for (auto& button : m_previousMouseDown) {
        button = false;
    }
}

void InputHandler::processEvent(const sf::Event& event) noexcept {
    switch (event.type) {
        case sf::Event::KeyPressed:
            m_keyDown[event.key.code] = true;
            break;
        case sf::Event::KeyReleased:
            m_keyDown[event.key.code] = false;
            break;
        case sf::Event::TextEntered: {
                m_textEntered.push(event.text.unicode);
                if (m_textEntered.size() > m_maxTextEnteredSize) {
                    m_textEntered.pop();
                }
            } break;
        case sf::Event::MouseButtonPressed:
            m_mouseDown[event.mouseButton.button] = true;
            break;
        case sf::Event::MouseButtonReleased:
            m_mouseDown[event.mouseButton.button] = false;
            break;
        case sf::Event::MouseMoved: {
                m_mouseX = event.mouseMove.x;
                m_mouseY = event.mouseMove.y;
            } break;
        default: break;
    }
}

bool InputHandler::getKeyDown(sf::Keyboard::Key key) const noexcept {
    return m_keyDown[key];
}

bool InputHandler::getMouseDown(sf::Mouse::Button button) const noexcept {
    return m_mouseDown[button];
}

bool InputHandler::getMouseClicked(sf::Mouse::Button button) const noexcept {
    return m_mouseDown[button] && !m_previousMouseDown[button];
}

sf::Vector2i InputHandler::getMousePosition() const noexcept {
    return sf::Vector2i(m_mouseX, m_mouseY);
}

std::queue<uint32_t> InputHandler::getTextEntered() noexcept {
    std::queue<uint32_t> returnQueue;
    std::swap(m_textEntered, returnQueue);
    return returnQueue;
}

void InputHandler::update() noexcept {
    std::copy(std::begin(m_mouseDown), std::end(m_mouseDown), std::begin(m_previousMouseDown));
}
