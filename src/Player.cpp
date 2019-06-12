#include <cmath>

#include "Game.hpp"
#include "Message.hpp"
#include "Player.hpp"

#include "PlayerMove.pb.h"

Player::Player(Game& game, const Character& character) : Character(character), m_controlsEnabled(true) {
    initializeSkeleton(&game);
}

Player::Player(const Player& other) : Character(other) {
    m_controlsEnabled = other.m_controlsEnabled;
    m_velocity = other.m_velocity;
    m_previousVelocity = other.m_previousVelocity;
}

Player::Player(Player&& other) : Character(std::move(other)) {
    m_controlsEnabled = other.m_controlsEnabled;
    m_velocity = std::move(other.m_velocity);
    m_previousVelocity = std::move(other.m_previousVelocity);
}

void Player::update(Game& game, const GameObjectCollection& gameObjectCollection, float deltaTime) noexcept {
    Character::update(game, gameObjectCollection, deltaTime);

    if (!m_controlsEnabled) {
        return;
    }

    auto& input = game.getInputHandler();
    auto& socket = game.getSocket();

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
            // Moving
            PlayerMove playerMove;
            playerMove.set_x(m_position.x);
            playerMove.set_y(m_position.y);
            playerMove.set_velocity_x(m_velocity.x);
            playerMove.set_velocity_y(m_velocity.y);

            socket.sendMessage(Message(MessageType::PlayerMove, playerMove));

            auto runAnimation = m_skeletonData->findAnimation("run");
            if (getAnimationState()->getCurrent(0)->getAnimation() != runAnimation) {
                getAnimationState()->setAnimation(0, runAnimation, true);
            }

            if (std::abs(m_velocity.x) > .01f) {
                setOrientation(m_velocity.x > 0.f);
            }
        } else {
            // Not moving
            PlayerStop playerStop;
            playerStop.set_x(m_position.x);
            playerStop.set_y(m_position.y);

            socket.sendMessage(Message(MessageType::PlayerStop, playerStop));

            auto idleAnimation = m_skeletonData->findAnimation("idle");
            if (getAnimationState()->getCurrent(0)->getAnimation() != idleAnimation) {
                getAnimationState()->setAnimation(0, idleAnimation, true);
            }
        }
    }

    m_position.x += m_velocity.x * deltaTime;
    m_position.y += m_velocity.y * deltaTime;

    m_drawable->skeleton->setPosition(m_position.x, m_position.y);

    auto view = game.getRenderWindow().getView();
    view.setCenter(m_position);
    game.getRenderWindow().setView(view);
}

void Player::setControlsEnabled(bool enabled) noexcept {
    m_controlsEnabled = enabled;
}
