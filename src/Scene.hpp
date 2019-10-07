#ifndef SCENE_HPP
#define SCENE_HPP

#include <functional>
#include <queue>

#include <SFML/Graphics.hpp>

#include "GameObject.hpp"
#include "GameObjectCollection.hpp"
#include "Message.hpp"
#include "MessageTypes.hpp"

class Game;
class Socket;

using MessageHandler = std::function<void(Game&, GameObjectCollection&, const Message&)>;
using MessageHandlerMap = std::unordered_map<MessageType, MessageHandler>;

class Scene {
public:
    virtual void initialize(Game&) = 0;

    bool handleMessage(Game&, const Message&);

    void update(Game&, float deltaTime) noexcept;

    void draw(sf::RenderWindow&) noexcept;

protected:
    GameObjectCollection m_objects;

    void addObject(const std::string& name, std::shared_ptr<GameObject>);
    void removeObject(const std::string& name);
    void addMessageHandler(MessageType, MessageHandler);

private:
    MessageHandlerMap m_handlers;
};

#endif // SCENE_HPP
