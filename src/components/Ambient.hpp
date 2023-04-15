#pragma once

#include <string>

#include "GameObject.hpp"
#include "Sprite.hpp"
#include "../managers/RenderManager.hpp"


class Ambient : public GameObject
{
private:

    Sprite*     sprite_;

public:

    Ambient(std::string id, Sprite* sprite, bool state = false);

    void update(RenderManager* renderer, int dt) override;
    bool clicked(int x, int y) override;

    std::string id();
    bool        state();
    void        state(bool new_state);
    Sprite*     sprite();

    void accept_click(Gameplay::GameplayState* handler, SDL_Event& event) override;
    void accept_over(Gameplay::GameplayState* handler, SDL_Event& event) override;
    void accept_drag(Gameplay::GameplayState* handler, SDL_Event& event) override;

};