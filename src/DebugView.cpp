#include "DebugView.hpp"
#include "Player.hpp"
#include "Map.hpp"
#include "Game.hpp"
#include "PlayerPool.hpp"

void DebugView::update(Game& game, GameObjectCollection& gameObjects, float) noexcept {
    m_boundingBoxes.clear();

    // Get bounding boxes of various game objects
    auto player = gameObjects.get<Player>("player");
    auto playerPool = gameObjects.get<PlayerPool>("playerPool");
    auto map = gameObjects.get<Map>("map");

    m_boundingBoxes.push_back(player->getBoundingBox());

    for (auto& otherPlayer : *playerPool) {
        m_boundingBoxes.push_back(otherPlayer.second->getBoundingBox());
    }

    // Find bounding boxes of chunks in view
    auto view = game.getRenderWindow().getView();

    auto topLeftViewCoordinates = sf::Vector2f(
        view.getCenter().x - view.getSize().x / 2.f,
        view.getCenter().y - view.getSize().y / 2.f);
    auto bottomRightViewCoordinates = sf::Vector2f(
        topLeftViewCoordinates.x + view.getSize().x,
        topLeftViewCoordinates.y + view.getSize().y);

    auto topLeftChunkId = map->getChunkId(topLeftViewCoordinates);
    auto bottomRightChunkId = map->getChunkId(bottomRightViewCoordinates);

    for (int i = topLeftChunkId.first; i <= bottomRightChunkId.first; i++) {
        for (int j = topLeftChunkId.second; j <= bottomRightChunkId.second; j++) {
            for (auto& chunkObject : map->getChunk({i, j})) {
                auto mapObject = map->getMapObject(chunkObject.id);

                m_boundingBoxes.emplace_back(sf::FloatRect(
                    chunkObject.x,
                    chunkObject.y - mapObject.height,
                    mapObject.width,
                    mapObject.height));
            }
        }
    }
}

float DebugView::getZIndex() const noexcept {
    return std::numeric_limits<float>::max();
}

void DebugView::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    sf::RectangleShape boxSprite;

    boxSprite.setFillColor(sf::Color::Transparent);
    boxSprite.setOutlineColor(sf::Color::Red);
    boxSprite.setOutlineThickness(1.f);

    for (auto& boundingBox : m_boundingBoxes) {
        boxSprite.setPosition(sf::Vector2f(boundingBox.left, boundingBox.top));
        boxSprite.setSize(sf::Vector2f(boundingBox.width, boundingBox.height));

        target.draw(boxSprite, states);
    }
}