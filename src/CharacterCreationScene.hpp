#ifndef CHARACTERCREATIONSCENE_HPP
#define CHARACTERCREATIONSCENE_HPP

#include <random>

#include <TGUI/TGUI.hpp>

#include "CharacterPreview.hpp"
#include "Scene.hpp"

class CharacterCreationScene : public Scene {
public:
    CharacterCreationScene();

    virtual void initialize(Game&) override;

private:
    std::mt19937 m_generator;
    std::shared_ptr<CharacterPreview> m_characterPreview;

    void createCharacter(
        Game&,
        tgui::EditBox::Ptr nameEditBox,
        std::shared_ptr<CharacterPreview>);
    void randomizeColor() noexcept;
};

#endif // CHARACTERCREATIONSCENE_HPP
