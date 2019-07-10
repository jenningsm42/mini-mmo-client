#include "Chatbox.hpp"
#include "Game.hpp"
#include "Player.hpp"
#include "PlayerPool.hpp"

#include "Chat.pb.h"

void Chatbox::load(Game& game, uint32_t playerId, const std::string& name) {
    m_playerId = playerId;
    m_name = name;

    auto& gui = game.getGui();

    m_input = tgui::EditBox::create();
    m_input->setSize({"&.width / 3", "24"});
    m_input->setPosition({"&.left", "&.bottom - height"});
    gui.add(m_input, "chatInput");

    m_chatbox = tgui::ChatBox::create();
    m_chatbox->setSize({"&.width / 3", "&.height / 6"});
    m_chatbox->setPosition({"&.left", "&.bottom - height - chatInput.height"});
    gui.add(m_chatbox);
}

void Chatbox::handleChatMessage(Game&, const GameObjectCollection& objects, const Message& message) noexcept {
    if (message.getType() != MessageType::ReceiveChatMessage) {
        std::cout << "Chatbox received wrong message type: " << static_cast<uint16_t>(message.getType()) << std::endl;
        return;
    }

    ReceiveChatMessage receiveChatMessage;
    message.getMessage(receiveChatMessage);

    std::string name;
    if (receiveChatMessage.player_id() == m_playerId) {
        name = m_name;
    }
    else {
        auto playerPool = std::dynamic_pointer_cast<PlayerPool>(objects.at("playerPool"));
        name = playerPool->getPlayerName(receiveChatMessage.player_id());
    }

    m_chatbox->addLine("[" + name + "] " + receiveChatMessage.msg());
}

void Chatbox::update(Game& game, const GameObjectCollection& objects, float) noexcept {
    auto& input = game.getInputHandler();
    auto& socket = game.getSocket();

    auto player = std::dynamic_pointer_cast<Player>(objects.at("player"));

    if (input.getKeyTapped(sf::Keyboard::Enter)) {
        if (m_input->isFocused()) {
            player->setControlsEnabled(true);

            auto text = m_input->getText().toAnsiString();
            if (text.size() > 0) {
                SendChatMessage sendChatMessage;
                sendChatMessage.set_msg(text);

                socket.sendMessage(Message(MessageType::SendChatMessage, sendChatMessage));

                m_input->setText("");
            }
        }
        else {
            player->setControlsEnabled(false);
        }

        m_input->setFocused(!m_input->isFocused());
    }
}

void Chatbox::draw(sf::RenderTarget&, sf::RenderStates) const {
}
