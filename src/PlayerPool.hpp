#ifndef PLAYERPOOL_HPP
#define PLAYERPOOL_HPP

#include <unordered_map>

#include "GameObject.hpp"
#include "Message.hpp"
#include "OtherPlayer.hpp"

#include "PlayerJoin.pb.h"
#include "PlayerMove.pb.h"

class PlayerPool : public GameObject {
public:
    void handleMessage(Game&, const Message&) noexcept;

    virtual void update(Game&, const GameObjectCollection&, float deltaTime) noexcept override;

    void addPlayer(
        Game& game,
        uint32_t id,
        float x,
        float y,
        float velocityX,
        float velocityY,
        const sf::Color& bodyColor,
        const sf::Color& shirtColor,
        const sf::Color& legsColor,
        const std::string& name) noexcept;

    void removePlayer(uint32_t id) noexcept;

    std::string getPlayerName(uint32_t id) noexcept;

private:
    std::unordered_map<uint32_t, OtherPlayer> m_players;

    virtual void draw(sf::RenderTarget&, sf::RenderStates) const override;

    void handlePlayersResponse(Game&, const PlayersResponse&);

    void handlePlayerJoin(Game&, const PlayerJoin&);
    void handlePlayerLeave(const PlayerLeave&);

    void handleOtherPlayerMove(const OtherPlayerMove&);
    void handleOtherPlayerStop(const OtherPlayerStop&);
};

#endif // PLAYERPOOL_HPP
