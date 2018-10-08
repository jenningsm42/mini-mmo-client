#include "Button.hpp"
#include "Scene.hpp"

Button::Button(float width, std::string&& name) {
    if (!m_font.loadFromFile("data/open_sans.ttf")) {
        throw new AssetMissingException("data/open_sans.ttf");
    }

    m_buttonText.setFont(m_font);
    const float height = static_cast<float>(m_buttonText.getCharacterSize()) * 1.2f;

    m_buttonText.setPosition(2.f, 0.f);
    m_buttonText.setString(std::move(name));

    m_textBox.setSize(sf::Vector2f(width, height));
    m_textBox.setFillColor(sf::Color::Black);
    m_textBox.setOutlineThickness(2.f);
    m_textBox.setOutlineColor(sf::Color::White);

    m_boundingBox = sf::FloatRect(0, 0, width, height);
}

void Button::draw(sf::RenderWindow& window) noexcept {
    window.draw(m_textBox, m_transform);
    window.draw(m_buttonText, m_transform * m_buttonText.getTransform());
}

void Button::setPosition(float x, float y) noexcept {
    m_transform = sf::Transform().translate(x, y);
    m_boundingBox = sf::FloatRect(x, y, m_boundingBox.width, m_boundingBox.height);
}

bool Button::getClicked(InputHandler& input) noexcept {
    if (input.getMouseClicked(sf::Mouse::Left)) {
        auto mousePosition = input.getMousePosition();
        return m_boundingBox.contains(mousePosition.x, mousePosition.y);
    }
    return false;
}
