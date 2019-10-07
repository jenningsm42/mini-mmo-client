#include <iostream>

#include "Chatbox.hpp"
#include "DebugView.hpp"
#include "Game.hpp"
#include "GameScene.hpp"
#include "Map.hpp"
#include "MessageTypes.hpp"
#include "Player.hpp"
#include "PlayerPool.hpp"

#include "PlayerJoin.pb.h"
#include "Chat.pb.h"
#include "Chunks.pb.h"

GameScene::GameScene(const Character& character) : m_character(character) {
}

void GameScene::initialize(Game& game) {
    // Send join request
    // TODO: Move this to the character selection screen in case server rejects request
    auto& socket = game.getSocket();

    JoinRequest joinRequest;
    joinRequest.set_character_id(m_character.getId());

    socket.sendMessage(Message(MessageType::JoinRequest, joinRequest));

    // Create game objects
    auto player = std::make_shared<Player>(game, m_character);
    auto playerPool = std::make_shared<PlayerPool>();
    auto chatbox = std::make_shared<Chatbox>();
    auto map = std::make_shared<Map>();

    chatbox->load(game, m_character.getId(), m_character.getName());

    map->loadMapObjects(game, "data/objects.txt");

    addObject("player", player);
    addObject("playerPool", playerPool);
    addObject("chatbox", chatbox);
    addObject("map", map);

    // Debug view for collision detection
    // addObject("debugView", std::make_shared<DebugView>());

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
            std::placeholders::_2,
            std::placeholders::_3));
    }

    addMessageHandler(MessageType::ReceiveChatMessage, std::bind(
        &Chatbox::handleChatMessage,
        chatbox,
        std::placeholders::_1,
        std::placeholders::_2,
        std::placeholders::_3));

    addMessageHandler(
        MessageType::ChunkResponse,
        [&](Game&, GameObjectCollection& gameObjects, const Message& message) {
            auto map = gameObjects.get<Map>("map");

            ChunkResponse chunkResponse;
            message.getMessage(chunkResponse);

            auto chunkId = std::make_pair<int, int>(chunkResponse.chunk_x(), chunkResponse.chunk_y());

            // Clear chunk first
            map->clearChunk(gameObjects, chunkId);

            for (auto& mapObject : chunkResponse.objects()) {
                map->addChunkObject(gameObjects, chunkId, ChunkObjectData{
                        mapObject.object_id(),
                        mapObject.x(),
                        mapObject.y(),
                });
            }
        });
}
