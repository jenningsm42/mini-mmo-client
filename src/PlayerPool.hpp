#ifndef PLAYERPOOL_HPP
#define PLAYERPOOL_HPP
#include <unordered_map>
#include "OtherPlayer.hpp"
#include "Message.hpp"

class PlayerPool {
    public:
        PlayerPool();

        void processMessage(const Message&) noexcept;
        void update(float deltaTime) noexcept;
        void draw(sf::RenderWindow&) noexcept;

        void addPlayer(uint32_t id, float x, float y, float velocityX, float velocityY,
                const sf::Color&, const std::string& name) noexcept;
        void removePlayer(uint32_t id) noexcept;

    private:
        std::unordered_map<uint32_t, OtherPlayer> m_players;
};

#endif
