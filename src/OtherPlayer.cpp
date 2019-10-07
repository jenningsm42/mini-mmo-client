#include "GameObjectCollection.hpp"
#include "Map.hpp"
#include "OtherPlayer.hpp"

OtherPlayer::OtherPlayer(Game& game, const Character& character) : Character(character) {
    initializeSkeleton(&game);
}

OtherPlayer::OtherPlayer(const OtherPlayer& other) : Character(other) {
    m_velocity = other.m_velocity;
}

OtherPlayer::OtherPlayer(OtherPlayer&& other) : Character(std::move(other)) {
    m_velocity = other.m_velocity;
}

void OtherPlayer::update(Game& game, GameObjectCollection& gameObjects, float deltaTime) noexcept {
    Character::update(game, gameObjects, deltaTime);

    auto map = gameObjects.get<Map>("map");
    setPosition(map->handleCollision(m_position, m_velocity * deltaTime, m_boundingBox));
}

void OtherPlayer::setVelocity(const sf::Vector2f& position, const sf::Vector2f& velocity) noexcept {
    if (std::abs(velocity.x) > .01f || std::abs(velocity.y) > .01f) {
        auto runAnimation = m_skeletonData->findAnimation("run");
        if (getAnimationState()->getCurrent(0)->getAnimation() != runAnimation) {
            getAnimationState()->setAnimation(0, runAnimation, true);
        }

        if (std::abs(velocity.x) > .01f) {
            setOrientation(velocity.x > 0.f);
        }
    } else {
        auto idleAnimation = m_skeletonData->findAnimation("idle");
        if (getAnimationState()->getCurrent(0)->getAnimation() != idleAnimation) {
            getAnimationState()->setAnimation(0, idleAnimation, true);
        }
    }

    setPosition(position);
    m_velocity = velocity;
}

OtherPlayer::operator std::string() const noexcept {
    return "<\"" + m_name + "\" (" +
        std::to_string(m_position.x) + ", " +
        std::to_string(m_position.y) + ")>";
}
