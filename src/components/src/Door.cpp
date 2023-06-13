#include "../Door.hpp"

#include <stdlib.h> // rand

#include "../../managers/AssetManager.hpp"
#include "../../managers/RenderManager.hpp"
#include "../logic/State.hpp"


// Door::Door() {}


Door::Door(YAML::Node data)
    : GameObject{data["id"].as<std::string>(), data["state"].as<bool>()}
{
    locked_ = data["locked"].as<bool>();
    target_ = data["target"].as<std::string>();
    observations_ = data["observations"].as<std::vector<std::string>>();
    locked_observations_ = data["locked_observations"].as<std::vector<std::string>>();
    key_id_ = data["key_id"].as<std::string>();
}


std::string Door::target() { return target_; }
bool Door::locked() { return locked_; }
void Door::unlock() { locked_ = false; }


std::string Door::observation()
{
    return observations_[rand() % observations_.size()];
}


std::string Door::locked_observation()
{
    return locked_observations_[rand() % locked_observations_.size()];
}


std::string Door::key_id() { return key_id_; }

void Door::accept_click(State* handler, int x, int y, bool r) { handler->visit_click(this, x, y, r); }
void Door::accept_over(State* handler, SDL_Event& event) { handler->visit_over(this, event); }
void Door::accept_drag(State* handler, SDL_Event& event) { handler->visit_drag(this, event); }


/********************************************************************************
 * Bitmap Door
********************************************************************************/

BitmapDoor::BitmapDoor(YAML::Node data)
    : Door{data} {}


void BitmapDoor::update(RenderManager* renderer, int dt) {}


bool BitmapDoor::clicked(float x, float y) { return false; }


/********************************************************************************
 * Sprite Door
********************************************************************************/

SpriteDoor::SpriteDoor(YAML::Node data, AssetManager* assets)
    : Door{data}
{
    sprite_ = assets->sprite(data["sprite"].as<std::string>());
    std::vector<int> position { data["position"].as<std::vector<int>>() };
    // scale item by its scale and also by the Room's scale.
    float hot_spot_scale { data["scale"].as<float>() };
    float room_scale { sprite_->scale() };
    sprite_->match_dimensions();
    sprite_->position(position[0] * room_scale + sprite_->x(), position[1] * room_scale);
    sprite_->scale(room_scale * hot_spot_scale);
    click_area_.add_vertices(data["click_area"].as<std::vector<std::vector<float>>>());
}


void SpriteDoor::update(RenderManager* renderer, int dt)
{
    sprite_->update(renderer, dt);
}


bool SpriteDoor::clicked(float x, float y)
{
    return click_area_.point_in_polygon(x, y) && state_;
}