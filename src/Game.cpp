#include "Game.hpp"

Game::Game() : m_window(sf::VideoMode(800, 600), "Client") {
}

Game::~Game() {
}

void Game::setScene(const std::shared_ptr<Scene>& scene) {
    m_nextScene = scene;
    m_nextScene->setGame(this);
}

void Game::run() {
    sf::Clock clock;
    while (m_window.isOpen()) {
        if (m_nextScene != nullptr) {
            m_currentScene = m_nextScene;
            m_nextScene = nullptr;
        }

        sf::Event event;
        m_inputHandler.update();
        while (m_window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                m_window.close();
            }
            m_inputHandler.processEvent(event);
        }

        this->processMessages();

        sf::Time elapsed = clock.restart();
        this->update(elapsed.asSeconds());

        m_window.clear();
        this->draw();
        m_window.display();
    }
}

void Game::processMessages() {
    if (m_currentScene != nullptr) {
        auto messages = m_socket.pollMessages();
        m_currentScene->processMessages(messages, m_socket);
    }
}

void Game::update(float deltaTime) {
    if (m_currentScene != nullptr) {
        m_currentScene->update(m_inputHandler, m_socket, deltaTime);
    }
}

void Game::draw() {
    if (m_currentScene != nullptr) {
        m_currentScene->draw(m_window);
    }
}
