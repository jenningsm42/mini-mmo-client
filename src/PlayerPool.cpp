#include "PlayerPool.hpp"
#include "OtherPlayer.hpp"
#include "Game.hpp"

#include <iostream>

void PlayerPool::handleMessage(
    Game& game,
    GameObjectCollection& gameObjects,
    const Message& message
) noexcept {
    switch (message.getType()) {
        case MessageType::PlayersResponse: {
            PlayersResponse playersResponse;
            message.getMessage(playersResponse);
            handlePlayersResponse(game, gameObjects, playersResponse);
            break;
        }
        case MessageType::PlayerJoin: {
            PlayerJoin playerJoin;
            message.getMessage(playerJoin);
            handlePlayerJoin(game, gameObjects, playerJoin);
            break;
        }
        case MessageType::PlayerLeave: {
            PlayerLeave playerLeave;
            message.getMessage(playerLeave);
            handlePlayerLeave(gameObjects, playerLeave);
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

void PlayerPool::update(Game&, GameObjectCollection&, float) noexcept {
}

std::string PlayerPool::getPlayerName(uint32_t id) noexcept {
    if (m_players.find(id) == m_players.end()) {
        return "";
    }

    return m_players.at(id)->getName();
}

void PlayerPool::draw(sf::RenderTarget&, sf::RenderStates) const {
}

void PlayerPool::addPlayer(
    Game& game,
    GameObjectCollection& gameObjects,
    uint32_t id,
    float x,
    float y,
    float velocityX,
    float velocityY,
    const sf::Color& bodyColor,
    const sf::Color& shirtColor,
    const sf::Color& legsColor,
    const std::string& name
) noexcept {
    auto character = Character(id, name, x, y, bodyColor, shirtColor, legsColor);
    m_players.emplace(id, std::make_shared<OtherPlayer>(game, character));
    m_players.at(id)->initializeSkeleton(&game);
    m_players.at(id)->setVelocity(sf::Vector2f(x, y), sf::Vector2f(velocityX, velocityY));

    gameObjects.add("otherPlayer-" + std::to_string(id), m_players.at(id));
}

void PlayerPool::removePlayer(GameObjectCollection& gameObjects, uint32_t id) noexcept {
    m_players.erase(id);

    gameObjects.remove("otherPlayer-" + std::to_string(id));
}

void PlayerPool::handlePlayersResponse(
    Game& game,
    GameObjectCollection& gameObjects,
    const PlayersResponse& playersResponse
) {
    for (auto& player : playersResponse.players()) {
        auto character = player.character();
        addPlayer(
            game,
            gameObjects,
            player.player_id(),
            character.x(),
            character.y(),
            player.velocity_x(),
            player.velocity_y(),
            sf::Color(character.body_color()),
            sf::Color(character.shirt_color()),
            sf::Color(character.legs_color()),
            character.name());
    }
}

void PlayerPool::handlePlayerJoin(
    Game& game,
    GameObjectCollection& gameObjects,
    const PlayerJoin& playerJoin
) {
    auto character = playerJoin.character();
    addPlayer(
        game,
        gameObjects,
        playerJoin.player_id(),
        character.x(),
        character.y(),
        0.f,
        0.f,
        sf::Color(character.body_color()),
        sf::Color(character.shirt_color()),
        sf::Color(character.legs_color()),
        character.name());
}

void PlayerPool::handlePlayerLeave(GameObjectCollection& gameObjects, const PlayerLeave& playerLeave) {
    removePlayer(gameObjects, playerLeave.player_id());
}

void PlayerPool::handleOtherPlayerMove(const OtherPlayerMove& otherPlayerMove) {
    try {
        m_players.at(otherPlayerMove.player_id())->setVelocity(
            sf::Vector2f(otherPlayerMove.x(), otherPlayerMove.y()),
            sf::Vector2f(otherPlayerMove.velocity_x(), otherPlayerMove.velocity_y()));
    }
    catch (std::out_of_range exception) {
        // Player doesn't exist - ignore message
        std::cout << "Received packet for non-existant player " <<
            otherPlayerMove.player_id() << std::endl;
    }
}

void PlayerPool::handleOtherPlayerStop(const OtherPlayerStop& otherPlayerStop) {
    try {
        m_players.at(otherPlayerStop.player_id())->setVelocity(
            sf::Vector2f(otherPlayerStop.x(), otherPlayerStop.y()),
            sf::Vector2f(0.f, 0.f));
    }
    catch (std::out_of_range exception) {
        // Player doesn't exist - ignore message
        std::cout << "Received packet for non-existant player " <<
            otherPlayerStop.player_id() << std::endl;
    }
}
