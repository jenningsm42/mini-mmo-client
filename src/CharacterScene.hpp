#ifndef CHARACTERSCENE_HPP
#define CHARACTERSCENE_HPP
#include "Button.hpp"
#include "Scene.hpp"
#include "Character.hpp"
#include "TextField.hpp"

class CharacterScene : public Scene {
    public:
        CharacterScene(Socket&);

        void processMessages(std::queue<Message>&, Socket&) override;
        void update(InputHandler&, Socket&, float deltaTime) noexcept override;
        void draw(sf::RenderWindow&) noexcept override;

    private:
        bool m_creatingCharacter;
        sf::Font m_font;

        std::vector<std::pair<Character, Button>> m_characterButtons;
        Button m_newCharacterButton;

        TextField m_characterNameInput;
        Button m_createCharacterButton;
        Button m_randomizeColorButton;
        sf::CircleShape m_characterPreview;

        void randomizeColor() noexcept;
};

#endif
