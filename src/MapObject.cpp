#include "Map.hpp"
#include "MapObject.hpp"

MapObject::MapObject(const ChunkObjectData& objectData, std::shared_ptr<sf::Texture> texture) {
    m_sprite.setTexture(*texture);
    m_sprite.setPosition(objectData.x, objectData.y - texture->getSize().y);
    m_zIndex = objectData.y;
}

void MapObject::update(Game&, GameObjectCollection&, float) noexcept {
}

float MapObject::getZIndex() const noexcept {
    return m_zIndex;
}

void MapObject::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    target.draw(m_sprite, states);
}
