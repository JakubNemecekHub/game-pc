#pragma once

#include <string>

#include "GameObject.hpp"

class RenderManager;


class Ambient : public GameObject
{
public:

    Ambient(std::string id, Sprite* sprite, bool state = false);
    Ambient(const Ambient& source) {};  // TO DO: do this

    void update(RenderManager* renderer, int dt) override;

    void accept_click(State* handler, Mouse::Status mouse) override;
    void accept_over(State* handler, Mouse::Status mouse) override;

};