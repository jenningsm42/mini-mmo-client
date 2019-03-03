#ifndef SCENE_HPP
#define SCENE_HPP

#include <functional>
#include <queue>

#include <SFML/Graphics.hpp>

#include "GameObject.hpp"
#include "Message.hpp"
#include "MessageTypes.hpp"

class Game;
class Socket;

using MessageHandler = std::function<void(Game&, const Message&)>;
using MessageHandlerMap = std::unordered_map<MessageType, MessageHandler>;

using GameObjectDrawOrder = std::map<int, std::vector<std::shared_ptr<GameObject>>>;

class Scene {
public:
    virtual void initialize(Game&) = 0;

    bool handleMessage(Game&, const Message&);

    void update(Game&, float deltaTime) noexcept;

    void draw(sf::RenderWindow&) noexcept;

    std::weak_ptr<GameObject> getObject(const std::string& name) noexcept;

protected:
    GameObjectCollection m_objects;

    void addObject(const std::string& name, std::shared_ptr<GameObject>, int zIndex = 0);
    void addMessageHandler(MessageType, MessageHandler);

private:
    MessageHandlerMap m_handlers;
    GameObjectDrawOrder m_objectsDrawOrder;
};

#endif // SCENE_HPP
