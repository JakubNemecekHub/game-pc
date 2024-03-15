#pragma once

#include <vector>

#include <yaml-cpp/yaml.h>

#include "GameObject.hpp"

class State;
class Polygon;


class WalkArea : public GameObject
{
// Shall only have Trigger Form
public:

    WalkArea(std::string id, Polygon polygon);

    Polygon* get()
    {
        Trigger* this_form = dynamic_cast<Trigger*>(form_.get());
        return this_form->get();
    }

    void update(RenderManager* renderer, int dt) override;

    void accept_click(State* handler, Mouse::Status mouse) override;
    void accept_over(State* handler, Mouse::Status mouse) override;

};
