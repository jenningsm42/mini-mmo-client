#include "LoginScene.hpp"
#include "Game.hpp"
#include "CharacterSelectionScene.hpp"

#include "Login.pb.h"
#include "Register.pb.h"

void LoginScene::initialize(Game& game) {
    auto& gui = game.getGui();

    auto usernameEditBox = tgui::EditBox::create();
    usernameEditBox->setSize({"30%", "5%"});
    usernameEditBox->setPosition({"&.width / 4 - width / 2", "&.height / 4 - height / 2"});
    usernameEditBox->setDefaultText("Username");
    gui.add(usernameEditBox, "usernameField");

    auto passwordEditBox = tgui::EditBox::create();
    passwordEditBox->setSize({"usernameField.width", "usernameField.height"});
    passwordEditBox->setPosition({"usernameField.left", "usernameField.bottom + 10"});
    passwordEditBox->setDefaultText("Password");
    passwordEditBox->setPasswordCharacter('*');
    gui.add(passwordEditBox, "passwordField");

    auto registerButton = tgui::Button::create("Register");
    registerButton->setSize({"usernameField.width / 2 - 5", "usernameField.height"});
    registerButton->setPosition({"passwordField.left", "passwordField.bottom + 10"});
    gui.add(registerButton, "registerButton");

    auto loginButton = tgui::Button::create("Login");
    loginButton->setSize({"usernameField.width / 2 - 5", "usernameField.height"});
    loginButton->setPosition({"registerButton.right + 10", "registerButton.top"});
    gui.add(loginButton);

    auto& socket = game.getSocket();

    // Button signals
    registerButton->connect("pressed", [&](
        tgui::EditBox::Ptr usernameEditBox, tgui::EditBox::Ptr passwordEditBox, Socket& socket
    ) {
        RegisterRequest registerRequest;
        registerRequest.set_username(usernameEditBox->getText().toAnsiString());
        registerRequest.set_password(passwordEditBox->getText().toAnsiString());

        socket.sendMessage(Message(MessageType::RegisterRequest, registerRequest));
    }, usernameEditBox, passwordEditBox, std::ref(socket));

    loginButton->connect("pressed", [&](
        tgui::EditBox::Ptr usernameEditBox, tgui::EditBox::Ptr passwordEditBox, Socket& socket
    ) {
        LoginRequest loginRequest;
        loginRequest.set_username(usernameEditBox->getText().toAnsiString());
        loginRequest.set_password(passwordEditBox->getText().toAnsiString());

        socket.sendMessage(Message(MessageType::LoginRequest, loginRequest));
    }, usernameEditBox, passwordEditBox, std::ref(socket));

    // Message handlers
    addMessageHandler(MessageType::LoginResponse, [&](Game& game, const Message& message) {
        LoginResponse loginResponse;
        message.getMessage(loginResponse);

        if (loginResponse.success()) {
            std::cout << "Logged in!" << std::endl;
            game.getSceneHandler().setScene(std::make_unique<CharacterSelectionScene>());
            return;
        } else {
            std::cout << loginResponse.error_message() << std::endl;
        }
    });

    addMessageHandler(MessageType::RegisterResponse, [&](Game&, const Message& message) {
        RegisterResponse registerResponse;
        message.getMessage(registerResponse);

        if (registerResponse.success()) {
            std::cout << "Registered!" << std::endl;
        } else {
            std::cout << registerResponse.error_message() << std::endl;
        }
    });
}
