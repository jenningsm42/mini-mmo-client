#ifndef GAMEOBJECT_HPP
#define GAMEOBJECT_HPP

#include <memory>
#include <unordered_map>

#include <SFML/Graphics/Drawable.hpp>

class Game;
class GameObjectCollection;

class GameObject;

class GameObject : public sf::Drawable {
public:
    virtual void update(Game&, GameObjectCollection&, float deltaTime) noexcept = 0;

    // Override getZIndex to add draw order sorting
    virtual float getZIndex() const noexcept {
        constexpr auto minimum = std::numeric_limits<float>::min();
        return minimum;
    };

private:
    virtual void draw(sf::RenderTarget&, sf::RenderStates) const = 0;
};


#endif // GAMEOBJECT_HPP
