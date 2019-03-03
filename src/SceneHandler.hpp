#ifndef SCENEHANDLER_HPP
#define SCENEHANDLER_HPP

#include "Scene.hpp"

using MessageQueue = std::queue<Message>;

class SceneHandler {
public:
    SceneHandler();
    SceneHandler(const SceneHandler&) = delete;
    ~SceneHandler();

    /* setScene stores the given scene to be switched to on the
     * next update call. If fade is true, then the screen will
     * fade to black first. */
    void setScene(std::unique_ptr<Scene>, bool fade = true) noexcept;

    void handleMessages(Game& game, MessageQueue&);

    /* update updates the current scene and switches to the next scene if
     * setScene was called. update will fade to black before switching
     * scenes if setScene was called. */
    void update(Game& game, float deltaTime);

    /* draw renders the current scene */
    void draw(sf::RenderWindow&) noexcept;

    /* drawFade renders the fade-to-black effect on top of
     * everything else */
    void drawFade(sf::RenderWindow&) noexcept;

private:
    std::unique_ptr<Scene> m_currentScene;
    std::unique_ptr<Scene> m_nextScene;
    bool m_fading;
    float m_alpha; // 0.0 = no black, 1.0 = full black screen
    sf::RectangleShape m_fadeBox; // Black rectangle overlay for fading

    const float m_fadeRate = 1.5f;

    void switchScenes(Game&);
};

#endif // SCENEHANDLER_HPP
