#include "Player.hpp"
#include "Message.hpp"
#include <cmath>

#include "PlayerMove.pb.h"

Player::Player() {
    m_sprite.setRadius(30.f);
    m_sprite.setFillColor(sf::Color::White);
    m_sprite.setPosition(300.f, 300.f);
}

void Player::update(InputHandler& input, Socket& socket, float deltaTime) noexcept {
    auto up = input.getKeyDown(sf::Keyboard::W);
    auto down = input.getKeyDown(sf::Keyboard::S);
    auto left = input.getKeyDown(sf::Keyboard::A);
    auto right = input.getKeyDown(sf::Keyboard::D);

    m_previousVelocity = m_velocity;
    m_velocity = sf::Vector2f();

    if ((up || down || left || right) && (up != down || left != right)) {
        float angle = 0.f;

        if (up == down) {
            angle = left? m_pi : 0.f;
        }
        else if (left == right) {
            angle = up? 3.f * m_pi / 2.f : m_pi / 2.f;
        }
        else if (up && left) {
            angle = 5.f * m_pi / 4.f;
        }
        else if (up && right) {
            angle = 7.f * m_pi / 4.f;
        }
        else if (down && left) {
            angle = 3.f * m_pi / 4.f;
        }
        else { // down and right
            angle = m_pi / 4.f;
        }

        m_velocity.x = m_speed * std::cos(angle);
        m_velocity.y = m_speed * std::sin(angle);
    }

    if (m_velocity != m_previousVelocity) {
        if (m_velocity != sf::Vector2f()) {
            PlayerMove playerMove;
            playerMove.set_current_x(m_sprite.getPosition().x);
            playerMove.set_current_y(m_sprite.getPosition().y);
            playerMove.set_velocity_x(m_velocity.x);
            playerMove.set_velocity_y(m_velocity.y);
            socket.sendMessage(Message(MessageType::PlayerMove, playerMove));
        } else {
            PlayerStop playerStop;
            playerStop.set_stopped_x(m_sprite.getPosition().x);
            playerStop.set_stopped_y(m_sprite.getPosition().y);
            socket.sendMessage(Message(MessageType::PlayerStop, playerStop));
        }
    }

    m_sprite.move(m_velocity.x * deltaTime, m_velocity.y * deltaTime);
}

void Player::draw(sf::RenderWindow& window) noexcept {
    window.draw(m_sprite);
}
