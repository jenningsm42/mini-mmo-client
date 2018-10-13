#include "GameScene.hpp"
#include "MessageTypes.hpp"
#include "PlayerJoin.pb.h"

GameScene::GameScene(Socket& socket, const Character& character)
    : m_player(character) {
    JoinRequest joinRequest;
    joinRequest.set_character_id(character.getId());
    socket.sendMessage(Message(MessageType::JoinRequest, joinRequest));
}

void GameScene::processMessages(std::queue<Message>& messages, Socket&) {
    while (!messages.empty()) {
        auto message = messages.front();
        switch (message.getType()) {
            case MessageType::PlayersResponse:
            case MessageType::OtherPlayerMove:
            case MessageType::OtherPlayerStop:
            case MessageType::PlayerJoin:
            case MessageType::PlayerLeave:
                m_playerPool.processMessage(message);
                break;
            default: break;
        }
        messages.pop();
    }
}

void GameScene::update(InputHandler& input, Socket& socket, float deltaTime) noexcept {
    m_playerPool.update(deltaTime);
    m_player.update(input, socket, deltaTime);
}

void GameScene::draw(sf::RenderWindow& window) noexcept {
    m_playerPool.draw(window);
    m_player.draw(window);
}
