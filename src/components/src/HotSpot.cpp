#include "../HotSpot.hpp"

#include <stdlib.h> // rand

#include "../logic/State.hpp"
#include "../managers/AssetManager.hpp"
#include "../managers/RenderManager.hpp"


HotSpot::HotSpot(YAML::Node data, AssetManager* assets)
    : GameObject{data["id"].as<std::string>(), data["state"].as<bool>()}
{
    // Texts
    observations_ = data["observations"].as<std::vector<std::string>>();
    use_observations_ = data["use_observations"].as<std::vector<std::string>>();
    // Form, could be RigidBody or Trigger
    // Sprite doesn't have to be in yaml file
    // Polygon on the other hand must be always present 
    Vertices vertices = data["click_area"].as<Vertices>();
    if (data["sprite"])
    {
        Sprite* sprite = assets->sprite(data["sprite"].as<std::string>());
        form_ = std::make_unique<RigidBody>(sprite, vertices);
        std::vector<float> position { data["position"].as<std::vector<float>>() };
        float hot_spot_scale { data["scale"].as<float>() };
        form_->position(position[0] + form_->x(), position[1]);
        form_->set_scale(hot_spot_scale);
    }
    else
    {
        form_ = std::make_unique<Trigger>(vertices);
    }
}


HotSpot::HotSpot(YAML::Node data, AssetManager* assets, float room_x, float room_y, float room_scale)
    : HotSpot(data, assets)
{
    form_->scale(room_scale);
    float x = form_->x() * room_scale + room_x;
    float y = form_->y() * room_scale + room_x;
    form_->position(x, y);
}


void HotSpot::update(RenderManager* renderer, int dt)
{
    form_->update(renderer, dt);
}


std::string HotSpot::observation()
{
    return observations_[rand() % observations_.size()];
}


std::string HotSpot::use_observation()
{
    return use_observations_[rand() % use_observations_.size()];
}


void HotSpot::accept_click(State* handler, Mouse::Status mouse) { handler->visit_click(this, mouse); }
void HotSpot::accept_over(State* handler, Mouse::Status mouse) { handler->visit_over(this, mouse); }
