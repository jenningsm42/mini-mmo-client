#include "Scene.hpp"

bool Scene::handleMessage(Game& game, const Message& message) {
    if (m_handlers.find(message.getType()) == m_handlers.end()) {
        return false;
    }

    auto handler = m_handlers[message.getType()];
    handler(game, m_objects, message);

    return true;
}

void Scene::update(Game& game, float deltaTime) noexcept {
    for (auto& object : m_objects) {
        object->update(game, m_objects, deltaTime);
    }
}

void Scene::draw(sf::RenderWindow& window) noexcept {
    m_objects.sort();

    for (auto& object : m_objects) {
        window.draw(*object);
    }
}

void Scene::addObject(const std::string& name, std::shared_ptr<GameObject> object) {
    m_objects.add(name, object);
}

void Scene::removeObject(const std::string &name) {
    m_objects.remove(name);
}

void Scene::addMessageHandler(MessageType type, MessageHandler handler) {
    m_handlers.emplace(type, handler);
}
