#pragma once

#include <vector>

#include <yaml-cpp/yaml.h>

#include "GameObject.hpp"
// #include "../math/Polygon.hpp" // To use Vertices typedef

class State;
class Polygon;


class WalkArea : public GameObject
{
// Shall only have Trigger Form
public:

    WalkArea(std::string id, std::vector<std::vector<float>> vertices);
    WalkArea(std::string id, std::vector<std::vector<float>> vertices, float room_x, float room_y, float room_scale);

    Polygon* get()
    {
        Trigger* this_form = dynamic_cast<Trigger*>(form_.get());
        return this_form->get();
    }

    void update(RenderManager* renderer, int dt) override;

    void accept_click(State* handler, Mouse::Status mouse) override;
    void accept_over(State* handler, Mouse::Status mouse) override;

};