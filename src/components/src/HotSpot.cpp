#include "../HotSpot.hpp"

#include <stdlib.h> // rand

#include "../managers/AssetManager.hpp"
#include "../managers/RenderManager.hpp"
#include "../logic/State.hpp"


HotSpot::HotSpot(YAML::Node data)
    : GameObject{data["id"].as<std::string>(), data["state"].as<bool>()}
{
    observations_ = data["observations"].as<std::vector<std::string>>();
    use_observations_ = data["use_observations"].as<std::vector<std::string>>();
}


std::string HotSpot::observation()
{
    return observations_[rand() % observations_.size()];
}


std::string HotSpot::use_observation()
{
    return use_observations_[rand() % use_observations_.size()];
}


void HotSpot::accept_click(State* handler, int x, int y, bool r) { handler->visit_click(this, x, y, r); }
void HotSpot::accept_over(State* handler, int x, int y) { handler->visit_over(this, x, y); }
void HotSpot::accept_drag(State* handler, SDL_Event& event) { handler->visit_drag(this, event); }


/********************************************************************************
 * Bitmap Hot Spot
********************************************************************************/

BitmapHotSpot::BitmapHotSpot(YAML::Node data)
    : HotSpot{data} {}


void BitmapHotSpot::update(RenderManager* renderer, int dt) {}


bool BitmapHotSpot::clicked(float x, float y) { return false; }


/********************************************************************************
 * Sprite Hot Spot
********************************************************************************/

SpriteHotSpot::SpriteHotSpot(YAML::Node data, AssetManager* assets)
    : HotSpot{data}
{
    sprite_ = assets->sprite(data["sprite"].as<std::string>());
    std::vector<float> position { data["position"].as<std::vector<float>>() };
    // scale item by its scale and also by the Room's scale.
    float hot_spot_scale { data["scale"].as<float>() };
    float room_scale { sprite_->scale() };
    sprite_->match_dimensions();
    sprite_->position(position[0] * room_scale + sprite_->x(), position[1] * room_scale);
    sprite_->scale(room_scale * hot_spot_scale);
    click_area_.add_vertices(data["click_area"].as<std::vector<std::vector<float>>>());
    // Scale the click area Polygon in the same way
    click_area_.scale(hot_spot_scale * room_scale);
    // click_area_.move(position[0] * room_scale + sprite_->x(), position[1] * room_scale);
    click_area_.move(position[0], position[1]);
}


void SpriteHotSpot::update(RenderManager* renderer, int dt)
{
    sprite_->update(renderer, dt);
    if ( debug_ )
    {
        renderer->submit(sprite_->position());
        renderer->submit(&click_area_);
        debug_ = false;
    }
}


bool SpriteHotSpot::clicked(float x, float y)
{
    return click_area_.point_in_polygon(x, y) && state_;
}