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
    void handleMessage(Game&, GameObjectCollection&, const Message&) noexcept;

    virtual void update(Game&, GameObjectCollection&, float deltaTime) noexcept override;

    void addPlayer(
        Game& game,
        GameObjectCollection&,
        uint32_t id,
        float x,
        float y,
        float velocityX,
        float velocityY,
        const sf::Color& bodyColor,
        const sf::Color& shirtColor,
        const sf::Color& legsColor,
        const std::string& name) noexcept;

    void removePlayer(GameObjectCollection&, uint32_t id) noexcept;

    std::string getPlayerName(uint32_t id) noexcept;

    std::unordered_map<uint32_t, std::shared_ptr<OtherPlayer>>::iterator begin();
    std::unordered_map<uint32_t, std::shared_ptr<OtherPlayer>>::iterator end();

private:
    std::unordered_map<uint32_t, std::shared_ptr<OtherPlayer>> m_players;

    virtual void draw(sf::RenderTarget&, sf::RenderStates) const override;

    void handlePlayersResponse(Game&, GameObjectCollection&, const PlayersResponse&);

    void handlePlayerJoin(Game&, GameObjectCollection&, const PlayerJoin&);
    void handlePlayerLeave(GameObjectCollection&, const PlayerLeave&);

    void handleOtherPlayerMove(const OtherPlayerMove&);
    void handleOtherPlayerStop(const OtherPlayerStop&);
};

#endif // PLAYERPOOL_HPP
