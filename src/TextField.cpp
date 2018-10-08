#include "TextField.hpp"
#include "Scene.hpp"

TextField::TextField(float width) : m_active(false) {
    if (!m_font.loadFromFile("data/open_sans.ttf")) {
        throw new AssetMissingException("data/open_sans.ttf");
    }

    m_inputText.setFont(m_font);
    const float height = static_cast<float>(m_inputText.getCharacterSize()) * 1.2f;

    m_inputText.setPosition(2.f, 0.f);

    m_textBox.setSize(sf::Vector2f(width, height));
    m_textBox.setFillColor(sf::Color::Black);
    m_textBox.setOutlineThickness(2.f);
    m_textBox.setOutlineColor(sf::Color::White);

    m_textCursor.setSize(sf::Vector2f(2.f, height * .8f));
    m_textCursor.setPosition(2.f, height * .1f);

    m_boundingBox = sf::FloatRect(0, 0, width, height);
}

void TextField::setActive(bool active, InputHandler* input) noexcept {
    if (active && !m_active) {
        m_cursorTimer = 0.f;

        // Reset recorded typing while text field was not active
        if (input != nullptr) {
            input->getTextEntered();
        }
    }
    m_active = active;
}

void TextField::update(InputHandler& input, float deltaTime) noexcept {
    if (m_active) {
        m_cursorTimer += deltaTime;
        if (m_cursorTimer >= m_cursorShowTime * 2.f) {
            m_cursorTimer = 0.f;
        }

        auto symbolsEntered = input.getTextEntered();
        while (!symbolsEntered.empty()) {
            auto symbol = symbolsEntered.front();
            if (symbol > 31 && symbol < 128) {
                m_input += static_cast<char>(symbol);
            } else if (symbol == 8 && !m_input.empty()) {
                // Backspace
                m_input.pop_back();
            }
            symbolsEntered.pop();
        }

        m_inputText.setString(m_input);
    }
}

void TextField::draw(sf::RenderWindow& window) noexcept {
    window.draw(m_textBox, m_transform);
    window.draw(m_inputText, m_transform * m_inputText.getTransform());
    if (m_active && m_cursorTimer <= m_cursorShowTime) {
        window.draw(m_textCursor, m_transform * m_textCursor.getTransform());
    }
}

void TextField::setPosition(float x, float y) noexcept {
    m_transform = sf::Transform().translate(x, y);
    m_boundingBox = sf::FloatRect(x, y, m_boundingBox.width, m_boundingBox.height);
}

sf::FloatRect TextField::getBoundingBox() const noexcept {
    return m_boundingBox;
}

std::string TextField::getInput() const noexcept {
    return m_input;
}
