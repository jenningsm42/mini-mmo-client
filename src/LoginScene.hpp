#ifndef LOGINSCENE_HPP
#define LOGINSCENE_HPP
#include "Button.hpp"
#include "Scene.hpp"
#include "TextField.hpp"

class LoginScene : public Scene {
    public:
        LoginScene();

        void processMessages(std::queue<Message>&, Socket&) override;
        void update(InputHandler&, Socket&, float deltaTime) noexcept override;
        void draw(sf::RenderWindow&) noexcept override;

    private:
        sf::Font m_font;

        sf::Text m_usernameText;
        sf::Text m_passwordText;

        TextField m_usernameInput;
        TextField m_passwordInput;

        Button m_registerButton;
        Button m_loginButton;
};

#endif
