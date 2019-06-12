#include "CharacterCreationScene.hpp"
#include "CharacterSelectionScene.hpp"
#include "ColorPickerWidget.hpp"
#include "Game.hpp"

#include "Characters.pb.h"

#include <iostream>

void CharacterCreationScene::initialize(Game& game) {
    // Character colors
    const std::vector<sf::Color> bodyColors = {
        sf::Color(0x226765ff),
        sf::Color(0xaa6b39ff),
        sf::Color(0x81a035ff),
        sf::Color(0x832c65ff),
    };

    const std::vector<sf::Color> shirtColors = {
        sf::Color(0xa7383dff),
        sf::Color(0xaa8b39ff),
        sf::Color(0x323875ff),
        sf::Color(0x378b2eff),
    };

    const std::vector<sf::Color> legsColors = {
        sf::Color(0x1a1d20ff),
        sf::Color(0x716848ff),
        sf::Color(0x603841ff),
    };

    // GUI elements
    auto& gui = game.getGui();

    auto bodyColorPicker = ColorPickerWidget::create("Body Color", bodyColors);
    bodyColorPicker->setPosition({"3 * &.width / 5", "30"});
    gui.add(bodyColorPicker, "bodyColorPicker");

    auto shirtColorPicker = ColorPickerWidget::create("Shirt Color", shirtColors);
    shirtColorPicker->setPosition({"bodyColorPicker.left", "bodyColorPicker.bottom + 20"});
    gui.add(shirtColorPicker, "shirtColorPicker");

    auto legsColorPicker = ColorPickerWidget::create("Legs Color", legsColors);
    legsColorPicker->setPosition({"bodyColorPicker.left", "shirtColorPicker.bottom + 20"});
    gui.add(legsColorPicker, "legsColorPicker");

    auto nameEditBox = tgui::EditBox::create();
    nameEditBox->setSize({"30%", "5%"});
    nameEditBox->setPosition({"bodyColorPicker.left", "legsColorPicker.bottom + 20"});
    nameEditBox->setDefaultText("Name");
    gui.add(nameEditBox, "nameEditBox");

    auto createButton = tgui::Button::create("Create!");
    createButton->setSize({"nameEditBox.width", "nameEditBox.height"});
    createButton->setPosition({"bodyColorPicker.left", "nameEditBox.bottom + 10"});
    gui.add(createButton, "createButton");

    // Game objects
    m_characterPreview = std::make_shared<Character>(
        0, "", 200.f, 400.f, bodyColors[0], shirtColors[0], legsColors[0]);
    m_characterPreview->initializeSkeleton(&game);
    m_characterPreview->setScale(3.f);

    addObject("characterPreview", m_characterPreview);

    // Signals
    bodyColorPicker->connect("colorChanged", &CharacterCreationScene::setBodyColor, this);
    shirtColorPicker->connect("colorChanged", &CharacterCreationScene::setShirtColor, this);
    legsColorPicker->connect("colorChanged", &CharacterCreationScene::setLegsColor, this);

    createButton->connect(
        "pressed",
        &CharacterCreationScene::createCharacter,
        this,
        std::ref(game),
        nameEditBox);

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

void CharacterCreationScene::createCharacter(Game& game, tgui::EditBox::Ptr nameEditBox) {
    auto& socket = game.getSocket();

    CreateCharacterRequest createCharacterRequest;
    createCharacterRequest.set_name(nameEditBox->getText().toAnsiString());
    createCharacterRequest.set_body_color(m_characterPreview->getBodyColor().toInteger());
    createCharacterRequest.set_shirt_color(m_characterPreview->getShirtColor().toInteger());
    createCharacterRequest.set_legs_color(m_characterPreview->getLegsColor().toInteger());

    socket.sendMessage(Message(
        MessageType::CreateCharacterRequest,
        createCharacterRequest));
}

void CharacterCreationScene::setBodyColor(sf::Uint32 colorInt) {
    m_characterPreview->setBodyColor(sf::Color(colorInt));
}

void CharacterCreationScene::setShirtColor(sf::Uint32 colorInt) {
    m_characterPreview->setShirtColor(sf::Color(colorInt));
}

void CharacterCreationScene::setLegsColor(sf::Uint32 colorInt) {
    m_characterPreview->setLegsColor(sf::Color(colorInt));
}
