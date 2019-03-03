#ifndef LOGINSCENE_HPP
#define LOGINSCENE_HPP

#include "Scene.hpp"

class LoginScene : public Scene {
public:
    virtual void initialize(Game&) override;

private:
    void onRegister();
    void onLogin();
};

#endif // LOGINSCENE_HPP
