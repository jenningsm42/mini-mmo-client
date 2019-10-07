#ifndef MAPOBJECT_HPP
#define MAPOBJECT_HPP

#include <SFML/Graphics.hpp>

#include "GameObject.hpp"

struct ChunkObjectData;

class MapObject : public GameObject {
public:
    MapObject(const ChunkObjectData&, std::shared_ptr<sf::Texture>);

    virtual void update(Game&, GameObjectCollection&, float deltaTime) noexcept override;

    virtual float getZIndex() const noexcept override;

private:
    sf::Sprite m_sprite;
    float m_zIndex;

    virtual void draw(sf::RenderTarget&, sf::RenderStates) const override;
};


#endif // MAPOBJECT_HPP
