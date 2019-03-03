#ifndef GAME_HPP
#define GAME_HPP

#include <memory>

#include <SFML/Graphics.hpp>
#include <TGUI/TGUI.hpp>

#include "AssetCache.hpp"
#include "InputHandler.hpp"
#include "SceneHandler.hpp"
#include "Scene.hpp"
#include "Socket.hpp"

class Game {
public:
    Game();
    Game(const Game&) = delete;
    ~Game();

    void run();

    AssetCache& getAssetCache() noexcept;
    SceneHandler& getSceneHandler() noexcept;
    InputHandler& getInputHandler() noexcept;
    Socket& getSocket() noexcept;
    sf::RenderWindow& getRenderWindow() noexcept;
    tgui::Gui& getGui() noexcept;

private:
    AssetCache m_assetCache;
    InputHandler m_inputHandler;
    SceneHandler m_sceneHandler;
    Socket m_socket;
    MessageQueue m_messageQueue;
    sf::RenderWindow m_window;
    tgui::Gui m_gui;

    void pollMessages();

    void update(float deltaTime);

    void writeCrashReport(const std::exception&);
};

#endif // GAME_HPP
