#include "CharacterCreationScene.hpp"
#include "CharacterSelectionScene.hpp"
#include "Game.hpp"

#include "Characters.pb.h"

#include <iostream>

CharacterCreationScene::CharacterCreationScene() : m_generator((std::random_device())()) {}

void CharacterCreationScene::initialize(Game& game) {
    auto& gui = game.getGui();

    auto nameEditBox = tgui::EditBox::create();
    nameEditBox->setSize({"30%", "5%"});
    nameEditBox->setPosition({"&.width / 2 - width / 2", "&.height / 3"});
    nameEditBox->setDefaultText("Name");
    gui.add(nameEditBox, "nameEditBox");

    auto createButton = tgui::Button::create("Create");
    createButton->setSize({"nameEditBox.width / 2 - 5", "nameEditBox.height"});
    createButton->setPosition({"nameEditBox.left", "nameEditBox.bottom + 10"});
    gui.add(createButton, "createButton");

    auto randomizeColorButton = tgui::Button::create("Randomize!");
    randomizeColorButton->setSize({"nameEditBox.width / 2 - 5", "nameEditBox.height"});
    randomizeColorButton->setPosition({"createButton.right + 10", "createButton.top"});
    gui.add(randomizeColorButton);

    // Game objects
    m_characterPreview = std::make_shared<CharacterPreview>();
    addObject("characterPreview", m_characterPreview);

    // Button signals
    createButton->connect(
        "pressed",
        &CharacterCreationScene::createCharacter,
        this,
        std::ref(game),
        nameEditBox,
        std::ref(m_characterPreview));

    randomizeColorButton->connect("pressed", &CharacterCreationScene::randomizeColor, this);

    // Message handlers
    addMessageHandler(MessageType::CreateCharacterResponse, [&](Game& game, const Message& message) {
        CreateCharacterResponse createCharacterResponse;
        message.getMessage(createCharacterResponse);

        if (createCharacterResponse.success()) {
            game.getSceneHandler().setScene(std::make_unique<CharacterSelectionScene>());
        } else {
            std::cout << createCharacterResponse.error_message() << std::endl;
        }
    });
}

void CharacterCreationScene::createCharacter(
    Game& game,
    tgui::EditBox::Ptr nameEditBox,
    std::shared_ptr<CharacterPreview> preview
) {
    auto& socket = game.getSocket();

    CreateCharacterRequest createCharacterRequest;
    createCharacterRequest.set_name(nameEditBox->getText().toAnsiString());
    createCharacterRequest.set_color(preview->getColor().toInteger());

    socket.sendMessage(Message(
        MessageType::CreateCharacterRequest,
        createCharacterRequest));
}

void CharacterCreationScene::randomizeColor() noexcept {
    auto color = sf::Color(m_generator());
    color.a = 0xff;
    m_characterPreview->setColor(color);
}
