#include "PlayerPool.hpp"
#include "OtherPlayer.hpp"

#include <iostream>

void PlayerPool::handleMessage(Game&, const Message& message) noexcept {
    switch (message.getType()) {
        case MessageType::PlayersResponse: {
            PlayersResponse playersResponse;
            message.getMessage(playersResponse);
            handlePlayersResponse(playersResponse);
            break;
        }
        case MessageType::PlayerJoin: {
            PlayerJoin playerJoin;
            message.getMessage(playerJoin);
            handlePlayerJoin(playerJoin);
            break;
        }
        case MessageType::PlayerLeave: {
            PlayerLeave playerLeave;
            message.getMessage(playerLeave);
            handlePlayerLeave(playerLeave);
            break;
        }
        case MessageType::OtherPlayerMove: {
            OtherPlayerMove otherPlayerMove;
            message.getMessage(otherPlayerMove);
            handleOtherPlayerMove(otherPlayerMove);
            break;
        }
        case MessageType::OtherPlayerStop: {
            OtherPlayerStop otherPlayerStop;
            message.getMessage(otherPlayerStop);
            handleOtherPlayerStop(otherPlayerStop);
            break;
        }
        default: break;
    }
}

void PlayerPool::update(
    Game& game,
    const GameObjectCollection& gameObjects,
    float deltaTime
) noexcept {
    for (auto& player : m_players) {
        player.second.update(game, gameObjects, deltaTime);
    }
}

std::string PlayerPool::getPlayerName(uint32_t id) noexcept {
    if (m_players.find(id) == m_players.end()) {
        return "";
    }

    return m_players.at(id).getName();
}

void PlayerPool::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    for (auto& player : m_players) {
        target.draw(player.second, states);
    }
}

void PlayerPool::addPlayer(
    uint32_t id,
    float x,
    float y,
    float velocityX,
    float velocityY,
    const sf::Color& color,
    const std::string& name
) noexcept {
    m_players.emplace(id, OtherPlayer(x, y, velocityX, velocityY, color, name));
}

void PlayerPool::removePlayer(uint32_t id) noexcept {
    m_players.erase(id);
}

void PlayerPool::handlePlayersResponse(const PlayersResponse& playersResponse) {
    for (auto& player : playersResponse.players()) {
        addPlayer(
            player.player_id(),
            player.x(),
            player.y(),
            player.velocity_x(),
            player.velocity_y(),
            sf::Color(player.color()),
            player.name());
    }
}

void PlayerPool::handlePlayerJoin(const PlayerJoin& playerJoin) {
    addPlayer(
        playerJoin.player_id(),
        playerJoin.x(),
        playerJoin.y(),
        0.f,
        0.f,
        sf::Color(playerJoin.color()),
        playerJoin.name());
}

void PlayerPool::handlePlayerLeave(const PlayerLeave& playerLeave) {
    removePlayer(playerLeave.player_id());
}

void PlayerPool::handleOtherPlayerMove(const OtherPlayerMove& otherPlayerMove) {
    try {
        m_players.at(otherPlayerMove.player_id()).setVelocity(
            otherPlayerMove.x(),
            otherPlayerMove.y(),
            otherPlayerMove.velocity_x(),
            otherPlayerMove.velocity_y());
    }
    catch (std::out_of_range exception) {
        // Player doesn't exist - ignore message
        std::cout << "Received packet for non-existant player " <<
            otherPlayerMove.player_id() << std::endl;
    }
}

void PlayerPool::handleOtherPlayerStop(const OtherPlayerStop& otherPlayerStop) {
    try {
        m_players.at(otherPlayerStop.player_id()).setVelocity(
            otherPlayerStop.x(),
            otherPlayerStop.y(),
            0.f,
            0.f);
    }
    catch (std::out_of_range exception) {
        // Player doesn't exist - ignore message
        std::cout << "Received packet for non-existant player " <<
            otherPlayerStop.player_id() << std::endl;
    }
}
