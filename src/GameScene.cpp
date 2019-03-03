#include "Game.hpp"
#include "GameScene.hpp"
#include "MessageTypes.hpp"
#include "Player.hpp"
#include "PlayerPool.hpp"

#include "PlayerJoin.pb.h"

GameScene::GameScene(const Character& character) : m_character(character) {
}

void GameScene::initialize(Game& game) {
    auto& socket = game.getSocket();

    JoinRequest joinRequest;
    joinRequest.set_character_id(m_character.getId());

    socket.sendMessage(Message(MessageType::JoinRequest, joinRequest));

    auto player = std::make_shared<Player>(m_character);
    auto playerPool = std::make_shared<PlayerPool>();

    MessageType playerPoolMessageTypes[] = {
        MessageType::PlayersResponse,
        MessageType::PlayerJoin,
        MessageType::PlayerLeave,
        MessageType::OtherPlayerMove,
        MessageType::OtherPlayerStop
    };

    for (auto& type : playerPoolMessageTypes) {
        addMessageHandler(type, std::bind(
            &PlayerPool::handleMessage,
            playerPool,
            std::placeholders::_1,
            std::placeholders::_2));
    }

    addObject("player", player);
    addObject("playerPool", playerPool);
}
