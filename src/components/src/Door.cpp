#include "../Door.hpp"

#include <stdlib.h> // rand

#include "../logic/State.hpp"
#include "../../managers/AssetManager.hpp"
#include "../../managers/RenderManager.hpp"


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
        std::vector<float> position { data["position"].as<std::vector<float>>() };
        float door_scale { data["scale"].as<float>() };
        form_->position(position[0] + form_->x(), position[1]);
        form_->set_scale(door_scale);
    }
    else
    {
        form_ = std::make_unique<Trigger>(vertices);
    }
}


Door::Door(YAML::Node data, AssetManager* assets, float room_x, float room_y, float room_scale)
    : Door(data, assets)
{
    form_->scale(room_scale);
    float x = form_->x() * room_scale + room_x;
    float y = form_->y() * room_scale + room_x;
    form_->position(x, y);
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
void Door::accept_drag(State* handler, Mouse::Status mouse) { handler->visit_drag(this, mouse); }