#ifndef SCENE_H
#define SCENE_H
#include <SFML/Graphics.hpp>

#include "InputHandler.hpp"
#include "Message.hpp"
#include "Socket.hpp"

class Game;

class AssetMissingException : public std::exception {
    public:
        AssetMissingException(const std::string& assetFile) : m_assetFile(assetFile) {}

        const char* what() const noexcept override {
            return ("Unable to load file: " + m_assetFile).c_str();
        }

    private:
        std::string m_assetFile;
};

class Scene {
    public:
        virtual void processMessages(std::queue<Message>&, Socket&) = 0;
        virtual void update(InputHandler&, Socket&, float deltaTime) noexcept = 0;
        virtual void draw(sf::RenderWindow&) noexcept = 0;

        void setGame(Game*);

    protected:
        Game* m_game;
};

#endif
