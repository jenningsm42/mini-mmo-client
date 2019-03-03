#ifndef GAMESCENE_HPP
#define GAMESCENE_HPP

#include "Scene.hpp"
#include "Character.hpp"

class GameScene : public Scene {
public:
    GameScene(const Character&);

    virtual void initialize(Game&) override;

private:
    Character m_character;
};

#endif // GAMESCENE_HPP
