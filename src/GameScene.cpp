#include "Game.hpp"
#include "GameScene.hpp"
#include "MessageTypes.hpp"
#include "Player.hpp"
#include "PlayerPool.hpp"
#include "Chatbox.hpp"

#include "PlayerJoin.pb.h"
#include "Chat.pb.h"

GameScene::GameScene(const Character& character) : m_character(character) {
}

void GameScene::initialize(Game& game) {
    auto& socket = game.getSocket();

    JoinRequest joinRequest;
    joinRequest.set_character_id(m_character.getId());

    socket.sendMessage(Message(MessageType::JoinRequest, joinRequest));

    auto player = std::make_shared<Player>(game, m_character);
    auto playerPool = std::make_shared<PlayerPool>();
    auto chatbox = std::make_shared<Chatbox>();

    // Game objects
    chatbox->load(game, m_character.getId(), m_character.getName());

    addObject("player", player);
    addObject("playerPool", playerPool);
    addObject("chatbox", chatbox);

    // Message handling
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
            std::ref(m_objects),
            std::placeholders::_2));
    }

    addMessageHandler(MessageType::ReceiveChatMessage, std::bind(
        &Chatbox::handleChatMessage,
        chatbox,
        std::placeholders::_1,
        std::ref(m_objects),
        std::placeholders::_2));
}
