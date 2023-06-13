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
    bool clicked(float x, float y) override;

    std::string id();
    bool        state();
    void        state(bool new_state);
    Sprite*     sprite();

    void accept_click(State* handler, int x, int y, bool r) override;
    void accept_over(State* handler, SDL_Event& event) override;
    void accept_drag(State* handler, SDL_Event& event) override;

};