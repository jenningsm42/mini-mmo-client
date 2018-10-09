#include "LoginScene.hpp"
#include "Game.hpp"
#include "CharacterScene.hpp"

#include "Login.pb.h"
#include "Register.pb.h"

LoginScene::LoginScene() : m_usernameInput(200.f), m_passwordInput(200.f),
    m_registerButton(200.f, "Register"), m_loginButton(200.f, "Login") {
    if (!m_font.loadFromFile("data/open_sans.ttf")) {
        throw new AssetMissingException("data/open_sans.ttf");
    }

    m_usernameText.setFont(m_font);
    m_passwordText.setFont(m_font);

    m_usernameText.setString("Username:");
    m_passwordText.setString("Password:");

    m_usernameText.setPosition(100.f, 100.f);
    m_passwordText.setPosition(100.f, 150.f);

    m_usernameInput.setPosition(270.f, 100.f);
    m_passwordInput.setPosition(270.f, 150.f);

    m_usernameInput.setActive(true);

    m_registerButton.setPosition(100.f, 200.f);
    m_loginButton.setPosition(330.f, 200.f);
}

void LoginScene::processMessages(std::queue<Message>& messages, Socket& socket) {
    while (!messages.empty()) {
        auto message = messages.front();
        switch (message.getType()) {
            case MessageType::LoginResponse: {
                LoginResponse loginResponse;
                message.getMessage(loginResponse);
                if (loginResponse.success()) {
                    std::cout << "Logged in!" << std::endl;
                    m_game->setScene(std::make_shared<CharacterScene>(socket));
                    return;
                } else {
                    std::cout << loginResponse.error_message() << std::endl;
                }
                break;
            }
            case MessageType::RegisterResponse: {
                RegisterResponse registerResponse;
                message.getMessage(registerResponse);
                if (registerResponse.success()) {
                    std::cout << "Registered!" << std::endl;
                } else {
                    std::cout << registerResponse.error_message() << std::endl;
                }
                break;
            }
            default: break;
        }
        messages.pop();
    }
}

void LoginScene::update(InputHandler& input, Socket& socket, float deltaTime) noexcept {
    if (input.getMouseClicked(sf::Mouse::Left)) {
        auto mousePosition = input.getMousePosition();
        if (m_usernameInput.getBoundingBox().contains(mousePosition.x, mousePosition.y)) {
            m_usernameInput.setActive(true, &input);
            m_passwordInput.setActive(false, &input);
        } else if (m_passwordInput.getBoundingBox().contains(mousePosition.x, mousePosition.y)) {
            m_passwordInput.setActive(true, &input);
            m_usernameInput.setActive(false, &input);
        }
    }

    if (m_registerButton.getClicked(input)) {
        RegisterRequest registerRequest;
        registerRequest.set_username(m_usernameInput.getInput());
        registerRequest.set_password(m_passwordInput.getInput());
        socket.sendMessage(Message(MessageType::RegisterRequest, registerRequest));
    }

    if (m_loginButton.getClicked(input)) {
        LoginRequest loginRequest;
        loginRequest.set_username(m_usernameInput.getInput());
        loginRequest.set_password(m_passwordInput.getInput());
        socket.sendMessage(Message(MessageType::LoginRequest, loginRequest));
    }

    m_usernameInput.update(input, deltaTime);
    m_passwordInput.update(input, deltaTime);
}

void LoginScene::draw(sf::RenderWindow& window) noexcept {
    window.draw(m_usernameText);
    window.draw(m_passwordText);
    m_usernameInput.draw(window);
    m_passwordInput.draw(window);
    m_loginButton.draw(window);
    m_registerButton.draw(window);
}
