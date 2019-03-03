#include "InputHandler.hpp"

InputHandler::InputHandler() noexcept {
    for (auto& key : m_keyDown) {
        key = false;
    }
    for (auto& key : m_previousKeyDown) {
        key = false;
    }

    for (auto& button : m_mouseDown) {
        button = false;
    }
    for (auto& button : m_previousMouseDown) {
        button = false;
    }
}

InputHandler::InputHandler(const InputHandler& other) {
    std::copy(
            std::begin(other.m_keyDown),
            std::end(other.m_keyDown),
            std::begin(m_keyDown));
    std::copy(
            std::begin(other.m_previousKeyDown),
            std::end(other.m_previousKeyDown),
            std::begin(m_previousKeyDown));

    std::copy(
            std::begin(other.m_mouseDown),
            std::end(other.m_mouseDown),
            std::begin(m_mouseDown));
    std::copy(
            std::begin(other.m_previousMouseDown),
            std::end(other.m_previousMouseDown),
            std::begin(m_previousMouseDown));

    m_mouseX = other.m_mouseX;
    m_mouseY = other.m_mouseY;
}

void InputHandler::processEvent(const sf::Event& event) noexcept {
    switch (event.type) {
        case sf::Event::KeyPressed:
            m_keyDown[event.key.code] = true;
            break;
        case sf::Event::KeyReleased:
            m_keyDown[event.key.code] = false;
            break;
        case sf::Event::MouseButtonPressed:
            m_mouseDown[event.mouseButton.button] = true;
            break;
        case sf::Event::MouseButtonReleased:
            m_mouseDown[event.mouseButton.button] = false;
            break;
        case sf::Event::MouseMoved: {
            m_mouseX = event.mouseMove.x;
            m_mouseY = event.mouseMove.y;
        }
            break;
        default:
            break;
    }
}

bool InputHandler::getKeyDown(sf::Keyboard::Key key) const noexcept {
    return m_keyDown[key];
}

bool InputHandler::getMouseDown(sf::Mouse::Button button) const noexcept {
    return m_mouseDown[button];
}

bool InputHandler::getKeyTapped(sf::Keyboard::Key key) const noexcept {
    return m_keyDown[key] && !m_previousKeyDown[key];
}

bool InputHandler::getMouseTapped(sf::Mouse::Button button) const noexcept {
    return m_mouseDown[button] && !m_previousMouseDown[button];
}

sf::Vector2i InputHandler::getMousePosition() const noexcept {
    return sf::Vector2i(m_mouseX, m_mouseY);
}

void InputHandler::update() noexcept {
    std::copy(std::begin(m_keyDown), std::end(m_keyDown), std::begin(m_previousKeyDown));
    std::copy(std::begin(m_mouseDown), std::end(m_mouseDown), std::begin(m_previousMouseDown));
}
