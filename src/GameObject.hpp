#ifndef GAMEOBJECT_HPP
#define GAMEOBJECT_HPP

#include <memory>
#include <unordered_map>

#include <SFML/Graphics/Drawable.hpp>

class Game;

class GameObject;

using GameObjectCollection = std::unordered_map<std::string, std::shared_ptr<GameObject>>;

class GameObject : public sf::Drawable {
public:
    virtual void update(Game&, const GameObjectCollection&, float deltaTime) noexcept = 0;

private:
    virtual void draw(sf::RenderTarget&, sf::RenderStates) const = 0;
};


#endif // GAMEOBJECT_HPP
