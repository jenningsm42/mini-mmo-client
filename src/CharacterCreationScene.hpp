#ifndef CHARACTERCREATIONSCENE_HPP
#define CHARACTERCREATIONSCENE_HPP

#include <random>

#include <TGUI/TGUI.hpp>

#include "Character.hpp"
#include "Scene.hpp"

class CharacterCreationScene : public Scene {
public:
    virtual void initialize(Game&) override;

private:
    std::shared_ptr<Character> m_characterPreview;

    void createCharacter(Game&, tgui::EditBox::Ptr nameEditBox);

    void setBodyColor(sf::Uint32);
    void setShirtColor(sf::Uint32);
    void setLegsColor(sf::Uint32);
};

#endif // CHARACTERCREATIONSCENE_HPP
