#include "CharacterCreationScene.hpp"
#include "CharacterSelectionScene.hpp"
#include "Game.hpp"
#include "GameScene.hpp"

#include "Characters.pb.h"

void CharacterSelectionScene::initialize(Game& game) {
    auto& gui = game.getGui();
    auto& socket = game.getSocket();

    socket.sendMessage(Message(MessageType::CharactersRequest));

    auto newCharacterButton = tgui::Button::create("New Character");
    newCharacterButton->setPosition({"&.width / 4 - width / 2", "&.height / 4 - height / 2"});
    gui.add(newCharacterButton, "newCharacterButton");

    // Button signals
    newCharacterButton->connect("pressed", [&]() {
        game.getSceneHandler().setScene(std::make_unique<CharacterCreationScene>());
    });

    // Message handlers
    addMessageHandler(MessageType::CharactersResponse, [&](Game& game, const Message& message) {
        CharactersResponse charactersResponse;
        message.getMessage(charactersResponse);
        auto characters = charactersResponse.characters();

        auto& gui = game.getGui();

        for (auto& characterButton : m_characterButtons) {
            gui.remove(characterButton.first);
        }

        m_characterButtons.clear();
        for (int i = 0; i < characters.size(); i++) {
            auto& character = characters[i];

            auto c = Character(
                character.character_id(),
                character.name(),
                character.x(),
                character.y(),
                sf::Color(character.color()));

            auto b = tgui::Button::create(character.name());

            if (i > 0) {
                auto previousButtonName = "characterButton" + std::to_string(i - 1);
                auto leftLayout = previousButtonName + ".left";
                auto topLayout = previousButtonName + ".bottom + 10";
                b->setPosition({leftLayout, topLayout});
            }
            else {
                b->setPosition({"newCharacterButton.left", "newCharacterButton.bottom + 20"});
            }

            auto buttonName = "characterButton" + std::to_string(i);
            gui.add(b, buttonName);

            m_characterButtons.push_back(std::make_pair(b, c));

            b->connect("pressed", [&](const Character c) {
                game.getSceneHandler().setScene(std::make_unique<GameScene>(c));
            }, c);
        }
    });
}
