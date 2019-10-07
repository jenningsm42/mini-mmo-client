#include <cmath>

#include "Game.hpp"
#include "Message.hpp"
#include "Player.hpp"

#include "PlayerMove.pb.h"
#include "Map.hpp"

constexpr float Pi = 3.14159f;

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
    m_velocity = other.m_velocity;
    m_previousVelocity = other.m_previousVelocity;
}

void Player::update(Game& game, GameObjectCollection& gameObjects, float deltaTime) noexcept {
    Character::update(game, gameObjects, deltaTime);

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
        float angle;

        if (up == down) {
            angle = left? Pi : 0.f;
        }
        else if (left == right) {
            angle = up? 3.f * Pi / 2.f : Pi / 2.f;
        }
        else if (up && left) {
            angle = 5.f * Pi / 4.f;
        }
        else if (up && right) {
            angle = 7.f * Pi / 4.f;
        }
        else if (down && left) {
            angle = 3.f * Pi / 4.f;
        }
        else { // down and right
            angle = Pi / 4.f;
        }

        m_velocity.x = Player::Speed * std::cos(angle);
        m_velocity.y = Player::Speed * std::sin(angle);
    }

    // Send player movement state to the server
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

    // Update position
    auto map = gameObjects.get<Map>("map");
    setPosition(map->handleCollision(m_position, m_velocity * deltaTime, m_boundingBox));

    // Center camera around player
    auto view = game.getRenderWindow().getView();
    view.setCenter(m_position);
    game.getRenderWindow().setView(view);
}

void Player::setControlsEnabled(bool enabled) noexcept {
    m_controlsEnabled = enabled;
}
