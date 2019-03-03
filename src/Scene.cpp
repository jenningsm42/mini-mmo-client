#include "Scene.hpp"

bool Scene::handleMessage(Game& game, const Message& message) {
    if (m_handlers.find(message.getType()) == m_handlers.end()) {
        return false;
    }

    auto handler = m_handlers[message.getType()];
    handler(game, message);

    return true;
}

void Scene::update(Game& game, float deltaTime) noexcept {
    for (auto& object : m_objects) {
        object.second->update(game, m_objects, deltaTime);
    }
}

void Scene::draw(sf::RenderWindow& window) noexcept {
    for (auto& objects : m_objectsDrawOrder) {
        for (auto& object : objects.second) {
            window.draw(*object);
        }
    }
}

std::weak_ptr<GameObject> Scene::getObject(const std::string& name) noexcept {
    if (m_objects.find(name) != m_objects.end()) {
        return m_objects[name];
    }

    return std::weak_ptr<GameObject>();
}

void Scene::addObject(const std::string& name, std::shared_ptr<GameObject> object, int zIndex) {
    m_objects.emplace(name, object);
    m_objectsDrawOrder[zIndex].push_back(object);
}

void Scene::addMessageHandler(MessageType type, MessageHandler handler) {
    m_handlers.emplace(type, handler);
}
