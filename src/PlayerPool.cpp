#include "PlayerPool.hpp"
#include "PlayerJoin.pb.h"
#include "PlayerMove.pb.h"

PlayerPool::PlayerPool() {
}

void PlayerPool::processMessage(const Message& message) noexcept {
    switch (message.getType()) {
        case MessageType::PlayersResponse: {
            PlayersResponse playersResponse;
            message.getMessage(playersResponse);
            std::cout << playersResponse.players_size() << std::endl;
            for (int i = 0; i < playersResponse.players_size(); i++) {
                auto player = playersResponse.players(i);
                addPlayer(
                    player.player_id(),
                    player.x(),
                    player.y(),
                    player.velocity_x(),
                    player.velocity_y());
            }
            break;
        }
        case MessageType::OtherPlayerMove: {
            OtherPlayerMove otherPlayerMove;
            message.getMessage(otherPlayerMove);
            try {
            m_players.at(otherPlayerMove.player_id()).setVelocity(
                otherPlayerMove.starting_x(),
                otherPlayerMove.starting_y(),
                otherPlayerMove.velocity_x(),
                otherPlayerMove.velocity_y());
            }
            catch (std::out_of_range exception) {
                // Player doesn't exist - ignore message
                std::cout << "Received packet for non-existant player " <<
                    otherPlayerMove.player_id() << std::endl;
            }
            break;
        }
        case MessageType::OtherPlayerStop: {
            OtherPlayerStop otherPlayerStop;
            message.getMessage(otherPlayerStop);
            try {
                m_players.at(otherPlayerStop.player_id()).setVelocity(
                    otherPlayerStop.stopped_x(),
                    otherPlayerStop.stopped_y(),
                    0.f,
                    0.f);
            }
            catch (std::out_of_range exception) {
                // Player doesn't exist - ignore message
                std::cout << "Received packet for non-existant player " <<
                    otherPlayerStop.player_id() << std::endl;
            }
            break;
        }
        case MessageType::PlayerJoin: {
            PlayerJoin playerJoin;
            message.getMessage(playerJoin);
            addPlayer(playerJoin.player_id(), playerJoin.x(), playerJoin.y(), 0.f, 0.f);
            break;
        }
        case MessageType::PlayerLeave: {
            PlayerLeave playerLeave;
            message.getMessage(playerLeave);
            removePlayer(playerLeave.player_id());
            break;
        }
        default: break;
    }
}

void PlayerPool::update(float deltaTime) noexcept {
    for (auto& player : m_players) {
        player.second.update(deltaTime);
    }
}

void PlayerPool::draw(sf::RenderWindow& window) noexcept {
    for (auto& player : m_players) {
        player.second.draw(window);
    }
}

void PlayerPool::addPlayer(uint32_t id, float x, float y, float velocityX, float velocityY) noexcept {
    m_players.emplace(id, OtherPlayer(x, y, velocityX, velocityY));
}

void PlayerPool::removePlayer(uint32_t id) noexcept {
    m_players.erase(id);
}
