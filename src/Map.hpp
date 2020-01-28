#ifndef MAP_HPP
#define MAP_HPP

#include <unordered_set>
#include <vector>

#include "GameObject.hpp"
#include "PairHash.hpp"

struct MapObjectData {
    uint32_t id;
    float width;
    float height;
    std::shared_ptr<sf::Texture> texture;
};

struct ChunkObjectData {
    uint32_t id;
    float x;
    float y;
};

class Map : public GameObject {
public:
    void loadMapObjects(Game&, const std::string& path);

    void addChunkObject(GameObjectCollection&, std::pair<int, int> chunkId, const ChunkObjectData &);
    void clearChunk(GameObjectCollection&, std::pair<int, int> chunkId);

    MapObjectData getMapObject(uint32_t id) const;

    // Return chunk coordinates from world coordinates
    std::pair<int, int> getChunkId(sf::Vector2f position) const;

    std::vector<ChunkObjectData> getChunk(std::pair<int, int>) const;

    sf::Vector2f handleCollision(
        sf::Vector2f oldPosition,
        sf::Vector2f velocity,
        sf::FloatRect boundingBox) const;

    virtual void update(Game&, GameObjectCollection&, float deltaTime) noexcept override;

private:
    static constexpr int ChunkSize = 8192;
    std::unordered_map<uint32_t, MapObjectData> m_mapObjects;
    std::unordered_map<std::pair<int, int>, std::vector<ChunkObjectData>, PairHash> m_chunks;
    std::unordered_set<std::pair<int, int>, PairHash> m_loadedChunks;
    std::unordered_set<std::pair<int, int>, PairHash> m_requestedChunks;

    void requestChunks(Game&, GameObjectCollection&);

    virtual void draw(sf::RenderTarget&, sf::RenderStates) const override;

    std::string formatChunkId(std::pair<int, int> chunkId) const;
    std::string formatObjectId(std::pair<int, int> chunkId, size_t objectIndex) const;
};


#endif // MAP_HPP
