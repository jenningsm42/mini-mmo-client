#ifndef CHARACTERSELECTIONSCENE_HPP
#define CHARACTERSELECTIONSCENE_HPP

#include <TGUI/TGUI.hpp>

#include "Scene.hpp"
#include "Character.hpp"

class CharacterSelectionScene : public Scene {
public:
    virtual void initialize(Game&) override;

private:
    std::vector<std::pair<tgui::Button::Ptr, Character>> m_characterButtons;
};

#endif // CHARACTERSELECTIONSCENE_HPP
