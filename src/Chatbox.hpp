#ifndef CHATBOX_HPP
#define CHATBOX_HPP

#include <TGUI/TGUI.hpp>

#include "GameObject.hpp"
#include "Message.hpp"

class Chatbox : public GameObject {
public:
    void load(Game&, uint32_t playerId, const std::string& name);

    void handleChatMessage(Game&, GameObjectCollection&, const Message&) noexcept;

    virtual void update(Game&, GameObjectCollection&, float deltaTime) noexcept override;

private:
    uint32_t m_playerId;
    std::string m_name;
    tgui::ChatBox::Ptr m_chatbox;
    tgui::EditBox::Ptr m_input;

    virtual void draw(sf::RenderTarget&, sf::RenderStates) const override;
};

#endif // CHATBOX_HPP
