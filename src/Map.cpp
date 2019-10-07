#include <fstream>
#include <iostream>
#include <sstream>

#include "Game.hpp"
#include "GameObjectCollection.hpp"
#include "Map.hpp"
#include "MapObject.hpp"
#include "Player.hpp"

#include "Chunks.pb.h"


void Map::loadMapObjects(Game& game, const std::string& path) {
    std::ifstream file(path);

    if (!file.is_open()) {
        throw AssetMissingException(path);
    }

    auto& cache = game.getAssetCache();

    std::string line;
    size_t lineCount = 0;
    while (std::getline(file, line)) {
        std::istringstream stream(line);
        lineCount++;

        if (line[0] == '#') {
            continue;
        }

        // object_id - aabb_width - aabbb_height - texture_path
        MapObjectData objectData;
        std::string texturePath;

        bool result = true;

        result &= bool(stream >> objectData.id);
        result &= bool(stream >> objectData.width);
        result &= bool(stream >> objectData.height);
        result &= bool(stream >> texturePath);

        if (!result) {
            std::cout << "Error parsing map object on line " << lineCount << std::endl;
            continue;
        }

        objectData.texture = cache.getTexture(texturePath);

        m_mapObjects.emplace(objectData.id, std::move(objectData));
    }

    file.close();
}

void Map::addChunkObject(
        GameObjectCollection& gameObjects,
        std::pair<int, int> chunkId,
        const ChunkObjectData& chunkObject
) {
    m_chunks[chunkId].push_back(chunkObject);

    auto mapObject = m_mapObjects.at(chunkObject.id);

    // Place into game object collection for proper z-ordering
    gameObjects.add(
        formatObjectId(chunkId, m_chunks.count(chunkId) - 1),
        std::make_shared<MapObject>(chunkObject, mapObject.texture));
}

void Map::clearChunk(GameObjectCollection& gameObjects, std::pair<int, int> chunkId) {
    if (m_chunks.find(chunkId) == m_chunks.end()) {
        return;
    }

    // Remove from game object collection first
    for (size_t i = 0; i < m_chunks.at(chunkId).size(); i++) {
        gameObjects.remove(formatObjectId(chunkId, i++));
    }

    m_chunks.erase(chunkId);
}

MapObjectData Map::getMapObject(uint32_t id) const {
    return m_mapObjects.at(id);
}

std::pair<int, int> Map::getChunkId(sf::Vector2f position) const {
    if (position.x < 0.f) {
        position.x -= Map::ChunkSize;
    }

    if (position.y < 0.f) {
        position.y -= Map::ChunkSize;
    }

    return std::make_pair<int, int>(
            int(position.x / Map::ChunkSize),
            int(position.y / Map::ChunkSize));
}

std::vector<ChunkObjectData> Map::getChunk(std::pair<int, int> chunkId) const {
    if (m_chunks.find(chunkId) != m_chunks.end()) {
        return m_chunks.at(chunkId);
    }

    return std::vector<ChunkObjectData>();
}

sf::Vector2f Map::handleCollision(
    sf::Vector2f oldPosition,
    sf::Vector2f velocity,
    sf::FloatRect boundingBox
) const {
    if (velocity == sf::Vector2f()) {
        return oldPosition;
    }

    // Get chunks surrounding player
    auto centerChunkId = getChunkId(oldPosition);
    std::vector<std::pair<int, int>> playerChunks = {
        {centerChunkId.first - 1, centerChunkId.second - 1},
        {centerChunkId.first - 1, centerChunkId.second},
        {centerChunkId.first - 1, centerChunkId.second + 1},

        {centerChunkId.first, centerChunkId.second - 1},
        centerChunkId,
        {centerChunkId.first, centerChunkId.second + 1},

        {centerChunkId.first + 1, centerChunkId.second - 1},
        {centerChunkId.first + 1, centerChunkId.second},
        {centerChunkId.first + 1, centerChunkId.second + 1},
    };

    // Cache bounding boxes of potential colliders
    std::vector<sf::FloatRect> worldBoundingBoxes;
    for (auto& chunkId : playerChunks) {
        if (m_chunks.find(chunkId) == m_chunks.end()) {
            continue;
        }

        for (auto& chunkObject : m_chunks.at(chunkId)) {
            if (m_mapObjects.find(chunkObject.id) == m_mapObjects.end()) {
                // Map object not loaded or not found
                continue;
            }

            // Make bounding box for given object
            auto mapObject = m_mapObjects.at(chunkObject.id);

            worldBoundingBoxes.emplace_back(sf::FloatRect(
                chunkObject.x,
                chunkObject.y - mapObject.height,
                mapObject.width,
                mapObject.height));
        }
    }

    // Store bounding box to position difference
    sf::Vector2f boundingBoxDifference = oldPosition - sf::Vector2f(boundingBox.left, boundingBox.top);

    // Test for horizontal collision first
    boundingBox.left += velocity.x;

    for (auto& bb : worldBoundingBoxes) {
        if (boundingBox.intersects(bb)) {
            if (velocity.x > 0.f) {
                // Move entity back to the left
                boundingBox.left = bb.left - boundingBox.width;
            } else {
                // Move entity back to the right
                boundingBox.left = bb.left + bb.width;
            }
        }
    }

    // Test for vertical collision
    boundingBox.top += velocity.y;

    for (auto& bb : worldBoundingBoxes) {
        if (boundingBox.intersects(bb)) {
            if (velocity.y > 0.f) {
                // Move entity back up
                boundingBox.top = bb.top - boundingBox.height;
            } else {
                // Move entity back down
                boundingBox.top = bb.top + bb.height;
            }
        }
    }

    return sf::Vector2f(boundingBox.left, boundingBox.top) + boundingBoxDifference;
}

void Map::update(Game& game, GameObjectCollection& gameObjects, float) noexcept {
    requestChunks(game, gameObjects);
}

void Map::requestChunks(Game& game, GameObjectCollection& gameObjects) {
    // Request surrounding chunks if not yet loaded
    auto player = gameObjects.get<Player>("player");

    auto playerChunkId = getChunkId(player->getPosition());

    std::vector<std::pair<int, int>> surroundingChunks = {
        {playerChunkId.first - 1, playerChunkId.second - 1},
        {playerChunkId.first - 1, playerChunkId.second},
        {playerChunkId.first - 1, playerChunkId.second + 1},

        {playerChunkId.first, playerChunkId.second - 1},
        playerChunkId,
        {playerChunkId.first, playerChunkId.second + 1},

        {playerChunkId.first + 1, playerChunkId.second - 1},
        {playerChunkId.first + 1, playerChunkId.second},
        {playerChunkId.first + 1, playerChunkId.second + 1},
    };

    auto& socket = game.getSocket();
    for (auto& chunkId : surroundingChunks) {
        if (m_loadedChunks.find(chunkId) == m_loadedChunks.end() &&
            m_requestedChunks.find(chunkId) == m_requestedChunks.end()) {
            ChunkRequest chunkRequest;
            chunkRequest.set_chunk_x(chunkId.first);
            chunkRequest.set_chunk_y(chunkId.second);

            socket.sendMessage(Message(MessageType::ChunkRequest, chunkRequest));

            // Don't want to request chunks every update tick
            m_requestedChunks.insert(chunkId);
        }
    }
}

void Map::draw(sf::RenderTarget&, sf::RenderStates) const {
}

std::string Map::formatChunkId(std::pair<int, int> chunkId) const {
    return "chunk-" + std::to_string(chunkId.first) + "-" + std::to_string(chunkId.second);
}

std::string Map::formatObjectId(std::pair<int, int> chunkId, size_t objectIndex) const {
    return formatChunkId(chunkId) + "-" + std::to_string(objectIndex);
}
