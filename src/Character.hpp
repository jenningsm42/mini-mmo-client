#ifndef CHARACTER_HPP
#define CHARACTER_HPP

#include <SFML/Graphics.hpp>
#include <spine/spine-sfml.h>

#include "GameObject.hpp"

class SkeletonNotLoadedException : public std::exception {
public:
    const char *what() const noexcept {
        return "Skeleton not loaded";
    }
};

class Character : public GameObject {
public:
    Character(
        uint32_t id,
        const std::string& name,
        float x,
        float y,
        const sf::Color& bodyColor,
        const sf::Color& shirtColor,
        const sf::Color& legsColor);
    Character(const Character&);
    Character(Character&&);

    void initializeSkeleton(Game* game = nullptr);
    void setOrientation(bool facingRight);
    void setScale(float scale);

    uint32_t getId() const noexcept;
    std::string getName() const noexcept;
    sf::Vector2f getPosition() const noexcept;
    spine::AnimationState* getAnimationState();
    spine::Skeleton* getSkeleton();

    void setBodyColor(const sf::Color&) noexcept;
    void setShirtColor(const sf::Color&) noexcept;
    void setLegsColor(const sf::Color&) noexcept;

    sf::Color getBodyColor() const noexcept;
    sf::Color getShirtColor() const noexcept;
    sf::Color getLegsColor() const noexcept;

    virtual void update(Game&, GameObjectCollection&, float deltaTime) noexcept override;

    virtual float getZIndex() const noexcept override;

protected:
    uint32_t m_id;
    std::string m_name;
    sf::Vector2f m_position;
    sf::Color m_bodyColor;
    sf::Color m_shirtColor;
    sf::Color m_legsColor;
    std::unique_ptr<spine::AnimationStateData> m_animationStateData;
    std::unique_ptr<spine::SkeletonDrawable> m_drawable;

    // Keep track of skeleton data for copy/move constructors
    std::shared_ptr<spine::SkeletonData> m_skeletonData;

private:
    static constexpr float SkeletonScale = .15f;

    virtual void draw(sf::RenderTarget&, sf::RenderStates) const override;
};

#endif // CHARACTER_HPP
