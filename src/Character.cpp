#include "Character.hpp"
#include "Game.hpp"
#include "Player.hpp"

#include <iostream>

Character::Character(
    uint32_t id,
    const std::string& name,
    float x,
    float y,
    const sf::Color& bodyColor,
    const sf::Color& shirtColor,
    const sf::Color& legsColor) :
    m_id(id),
    m_name(name),
    m_position(x, y),
    m_boundingBox(
        x - Character::AABBWidth / 2.f,
        y - Character::AABBHeight,
        Character::AABBWidth,
        Character::AABBHeight),
    m_bodyColor(bodyColor),
    m_shirtColor(shirtColor),
    m_legsColor(legsColor) {}

Character::Character(const Character& other) {
    m_id = other.m_id;
    m_name = other.m_name;
    m_position = other.m_position;
    m_boundingBox = other.m_boundingBox;
    m_bodyColor = other.m_bodyColor;
    m_shirtColor = other.m_shirtColor;
    m_legsColor = other.m_legsColor;

    if (other.m_skeletonData != nullptr) {
        m_skeletonData = other.m_skeletonData;
        initializeSkeleton();
    }
}

Character::Character(Character&& other) {
    m_id = other.m_id;
    m_name = std::move(other.m_name);
    m_position = other.m_position;
    m_boundingBox = other.m_boundingBox;
    m_bodyColor = other.m_bodyColor;
    m_shirtColor = other.m_shirtColor;
    m_legsColor = other.m_legsColor;

    if (other.m_skeletonData != nullptr) {
        m_animationStateData = std::move(other.m_animationStateData);
        m_drawable = std::move(other.m_drawable);
    }
}

void Character::initializeSkeleton(Game* game) {
    if (m_skeletonData == nullptr && game != nullptr) {
        m_skeletonData = game->getAssetCache().getSkeleton("avatar", "avatar.atlas");
    }
    else if (m_skeletonData == nullptr && game == nullptr) {
        std::cerr << "Cannot initialize character skeleton" << std::endl;
        return;
    }

    m_animationStateData = std::make_unique<spine::AnimationStateData>(m_skeletonData.get());
    m_animationStateData->setDefaultMix(.1f);

    m_drawable = std::make_unique<spine::SkeletonDrawable>(
        m_skeletonData.get(), m_animationStateData.get());
    m_drawable->setUsePremultipliedAlpha(true);

    auto skeleton = m_drawable->skeleton;
    skeleton->setToSetupPose();

    skeleton->setPosition(m_position.x, m_position.y);

    skeleton->setScaleX(Character::SkeletonScale);
    skeleton->setScaleY(Character::SkeletonScale);

    skeleton->updateWorldTransform();

    setBodyColor(m_bodyColor);
    setShirtColor(m_shirtColor);
    setLegsColor(m_legsColor);

    getAnimationState()->setAnimation(0, "idle", true);
}

void Character::setOrientation(bool facingRight) {
    float scale = std::abs(getSkeleton()->getScaleX());
    getSkeleton()->setScaleX(facingRight? scale : -scale);
    getSkeleton()->updateWorldTransform();
}

void Character::setScale(float scale) {
    // Keep left-right orientation when scaling
    float currentScale = getSkeleton()->getScaleX();

    float newScale = Character::SkeletonScale * scale;
    getSkeleton()->setScaleX(currentScale >= 0? newScale : -newScale);
    getSkeleton()->setScaleY(newScale);

    getSkeleton()->updateWorldTransform();
}

uint32_t Character::getId() const noexcept {
    return m_id;
}

std::string Character::getName() const noexcept {
    return m_name;
}

spine::AnimationState* Character::getAnimationState() {
    if (m_drawable == nullptr) {
        throw SkeletonNotLoadedException();
    }

    return m_drawable->state;
}

spine::Skeleton* Character::getSkeleton() {
    if (m_drawable == nullptr) {
        throw SkeletonNotLoadedException();
    }

    return m_drawable->skeleton;
}

void Character::setPosition(const sf::Vector2f& position) noexcept {
    m_position = position;

    // Update skeleton
    if (m_drawable != nullptr) {
        m_drawable->skeleton->setPosition(m_position.x, m_position.y);
        m_drawable->skeleton->updateWorldTransform();
    }

    // Update bounding box
    m_boundingBox.left = m_position.x - Character::AABBWidth / 2.f;
    m_boundingBox.top = m_position.y - Character::AABBHeight;
}

sf::Vector2f Character::getPosition() const noexcept {
    return m_position;
}

sf::FloatRect Character::getBoundingBox() const noexcept {
    return m_boundingBox;
}

void Character::setBodyColor(const sf::Color& color) noexcept {
    m_bodyColor = color;

    if (m_drawable != nullptr) {
        auto slot = m_drawable->skeleton->findSlot("Head");
        slot->getColor().set(
                m_bodyColor.r / 255.f,
                m_bodyColor.g / 255.f,
                m_bodyColor.b / 255.f,
                1.f);
    }
}

void Character::setShirtColor(const sf::Color& color) noexcept {
    m_shirtColor = color;

    if (m_drawable != nullptr) {
        auto slot = m_drawable->skeleton->findSlot("Torso");
        slot->getColor().set(
                m_shirtColor.r / 255.f,
                m_shirtColor.g / 255.f,
                m_shirtColor.b / 255.f,
                1.f);
    }
}

void Character::setLegsColor(const sf::Color& color) noexcept {
    m_legsColor = color;

    if (m_drawable != nullptr) {
        auto legLeftSlot = m_drawable->skeleton->findSlot("leg left");
        auto legRightSlot = m_drawable->skeleton->findSlot("leg right");
        legLeftSlot->getColor().set(
                m_legsColor.r / 255.f,
                m_legsColor.g / 255.f,
                m_legsColor.b / 255.f,
                1.f);
        legRightSlot->getColor().set(
                m_legsColor.r / 255.f,
                m_legsColor.g / 255.f,
                m_legsColor.b / 255.f,
                1.f);
    }
}

sf::Color Character::getBodyColor() const noexcept {
    return m_bodyColor;
}

sf::Color Character::getShirtColor() const noexcept {
    return m_shirtColor;
}

sf::Color Character::getLegsColor() const noexcept {
    return m_legsColor;
}

void Character::update(Game&, GameObjectCollection&, float deltaTime) noexcept {
    if (m_drawable != nullptr) {
        m_drawable->update(deltaTime);
    }
}

float Character::getZIndex() const noexcept {
    return m_position.y;
}

void Character::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    if (m_drawable != nullptr) {
        target.draw(*m_drawable.get(), states);
    }
}
