#include "../Door.hpp"

#include <stdlib.h> // rand

#include "../logic/State.hpp"
#include "../../managers/AssetManager.hpp"
#include "../../managers/RenderManager.hpp"
#include "../../math/Vector2D.hpp"


Door::Door(YAML::Node data, AssetManager* assets)
    : GameObject{data["id"].as<std::string>(), data["state"].as<bool>()}
{
    locked_ = data["locked"].as<bool>();
    target_ = data["target"].as<std::string>();
    observations_ = data["observations"].as<std::vector<std::string>>();
    locked_observations_ = data["locked_observations"].as<std::vector<std::string>>();
    key_id_ = data["key_id"].as<std::string>();
    // Form, could be RigidBody or Trigger
    // Sprites does't have to be in yaml file,
    // polygon on the other hand must be present always
    Vertices vertices = data["click_area"].as<Vertices>();
    if (data["sprite"])
    {
        Sprite* sprite = assets->sprite(data["sprite"].as<std::string>());
        form_ = std::make_unique<RigidBody>(sprite, vertices);
        float door_scale  { data["scale"].as<float>()       };
        Vector2D position { data["position"].as<Vector2D>() };
        form_->position(position);
        form_->set_scale(door_scale);
    }
    else
    {
        form_ = std::make_unique<Trigger>(vertices);
    }
    // camera setting
    zoom_ = data["camera"]["zoom"].as<float>();
    position_ = data["camera"]["position"].as<Vector2D>();
}


void Door::update(RenderManager* renderer, int dt)
{
    form_->update(renderer, dt);
}


std::string Door::observation()
{
    return observations_[rand() % observations_.size()];
}


std::string Door::locked_observation()
{
    return locked_observations_[rand() % locked_observations_.size()];
}


void Door::accept_click(State* handler, Mouse::Status mouse) { handler->visit_click(this, mouse); }
void Door::accept_over(State* handler, Mouse::Status mouse) { handler->visit_over(this, mouse); }

std::tuple<float, Vector2D> Door::camera()
{
    return std::make_tuple(zoom_, position_);
}
