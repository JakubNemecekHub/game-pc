#pragma once

#include <string>

#include "GameObject.hpp"
#include "Sprite.hpp"


class Ambient : public GameObject
{
private:

    std::string id_;
    bool        state_;
    Sprite*     sprite_;

public:

    Ambient(std::string id, Sprite* sprite, bool state = false);

    std::string id();
    bool        state();
    void        state(bool new_state);
    Sprite*     sprite();
    bool        point_in(int x, int y);

    void accept_click(Gameplay::GameplayState* handler, SDL_Event& event) override;
    void accept_over(Gameplay::GameplayState* handler, SDL_Event& event) override;
    void accept_drag(Gameplay::GameplayState* handler, SDL_Event& event) override;

};