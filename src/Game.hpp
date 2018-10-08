#include <memory>

#include <SFML/Graphics.hpp>

#include "InputHandler.hpp"
#include "Scene.hpp"
#include "Socket.hpp"

class Game {
    public:
        Game();
        ~Game();

        void setScene(const std::shared_ptr<Scene>&);

        void run();

    private:
        std::shared_ptr<Scene> m_currentScene;
        std::shared_ptr<Scene> m_nextScene;
        Socket m_socket;
        InputHandler m_inputHandler;
        sf::RenderWindow m_window;

        void processMessages();
        void update(float deltaTime);
        void draw();
};
