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
        std::vector<float> position { data["position"].as<std::vector<float>>() };  // TO DO: load as Vector2D
        float hot_spot_scale { data["scale"].as<float>() };
        form_->position(Vector2D{position[0], position[1]});
        form_->set_scale(hot_spot_scale);
    }
    else
    {
        form_ = std::make_unique<Trigger>(vertices);
    }
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
