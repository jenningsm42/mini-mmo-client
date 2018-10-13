#include "CharacterScene.hpp"
#include <random>
#include "Game.hpp"
#include "GameScene.hpp"
#include "Characters.pb.h"

CharacterScene::CharacterScene(Socket& socket) :
    m_creatingCharacter(false),
    m_newCharacterButton(200.f, "New Character"),
    m_characterNameInput(200.f),
    m_createCharacterButton(200.f, "Create"),
    m_randomizeColorButton(100.f, "Randomize!") {
    if (!m_font.loadFromFile("data/open_sans.ttf")) {
        throw new AssetMissingException("data/open_sans.ttf");
    }

    m_characterPreview.setRadius(30.f);
    m_characterPreview.setPosition(285.f, 300.f);

    m_characterNameInput.setPosition(200.f, 400.f);
    m_createCharacterButton.setPosition(200.f, 450.f);
    m_randomizeColorButton.setPosition(100.f, 330.f);

    m_newCharacterButton.setPosition(250.f, 30.f);

    randomizeColor();

    socket.sendMessage(Message(MessageType::CharactersRequest));
}

void CharacterScene::processMessages(std::queue<Message>& messages, Socket&) {
    while (!messages.empty()) {
        auto message = messages.front();
        switch (message.getType()) {
            case MessageType::CharactersResponse: {
                CharactersResponse charactersResponse;
                message.getMessage(charactersResponse);
                auto characters = charactersResponse.characters();
                m_characterButtons.clear();
                for (int i = 0; i < characters.size(); i++) {
                    auto& character = characters[i];

                    auto c = Character(
                        character.character_id(),
                        character.x(),
                        character.y(),
                        sf::Color(character.color()));

                    auto b = Button(200.f, std::string(character.name()));
                    b.setPosition(30.f, 30.f + 50.f * i);

                    m_characterButtons.push_back(std::make_pair(c, b));
                }
                break;
            }
            case MessageType::CreateCharacterResponse: {
                CreateCharacterResponse createCharacterResponse;
                message.getMessage(createCharacterResponse);
                if (createCharacterResponse.success()) {
                    auto c = Character(
                        createCharacterResponse.character_id(),
                        createCharacterResponse.x(),
                        createCharacterResponse.y(),
                        m_characterPreview.getFillColor());

                    auto b = Button(200.f, std::string(m_characterNameInput.getInput()));
                    b.setPosition(30.f, 30.f + 50.f * m_characterButtons.size());

                    m_characterButtons.push_back(std::make_pair(c, b));

                    m_characterNameInput.clearInput();
                    m_creatingCharacter = false;
                } else {
                    std::cout << createCharacterResponse.error_message() << std::endl;
                }
                break;
            }
            default: break;
        }
        messages.pop();
    }
}

void CharacterScene::update(InputHandler& input, Socket& socket, float deltaTime) noexcept {
    if (m_creatingCharacter) {
        if (m_createCharacterButton.getClicked(input)) {
            CreateCharacterRequest createCharacterRequest;
            createCharacterRequest.set_name(m_characterNameInput.getInput());
            createCharacterRequest.set_color(m_characterPreview.getFillColor().toInteger());
            socket.sendMessage(Message(
                MessageType::CreateCharacterRequest,
                createCharacterRequest));
        }

        if (m_randomizeColorButton.getClicked(input)) {
            randomizeColor();
        }

        m_characterNameInput.update(input, deltaTime);
    } else {
        if (m_newCharacterButton.getClicked(input)) {
            m_creatingCharacter = true;
            m_characterNameInput.setActive(true, &input);
        }

        for (auto& characterButton : m_characterButtons) {
            if (characterButton.second.getClicked(input)) {
                m_game->setScene(std::make_shared<GameScene>(socket, characterButton.first));
                return;
            }
        }
    }
}

void CharacterScene::draw(sf::RenderWindow& window) noexcept {
    if (m_creatingCharacter) {
        m_characterNameInput.draw(window);
        m_createCharacterButton.draw(window);
        m_randomizeColorButton.draw(window);

        window.draw(m_characterPreview);
    } else {
        m_newCharacterButton.draw(window);

        for (auto& characterButton : m_characterButtons) {
            characterButton.second.draw(window);
        }
    }
}

void CharacterScene::randomizeColor() noexcept {
    std::mt19937 generator((std::random_device())());
    auto color = sf::Color(generator());
    color.a = 0xff;
    m_characterPreview.setFillColor(color);
}
