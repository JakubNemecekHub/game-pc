#include "../Item.hpp"

#include <stdlib.h> // rand


Item::Item(YAML::Node data, AssetManager* assets)
{
    id_ = data["id"].as<std::string>();
    // Default state and locked is false
    state_ = false;
    lock_ = false;
    // Texts
    observations_ = data["observations"].as<std::vector<std::string>>();
    pick_observation_ = data["pick_observation"].as<std::string>();
    // Load click area polygon
    click_area_.add_vertices(data["click_area"].as<std::vector<std::vector<int>>>());
    // Load texture
    sprite_ = assets->sprite(id_);  // TO DO: log error if assets return nullptr
}


bool Item::clicked(int x, int y)
{
    return click_area_.point_in_polygon(x, y);
}


std::string Item::get_observation()
{
    return observations_[rand() % observations_.size()];
}


std::string Item::get_pick_observation()
{
    return pick_observation_;
}


std::string Item::id() { return id_; }
bool Item::state() { return state_; }
void Item::state(bool new_state) { state_ = new_state; }
bool Item::lock() { return lock_; }
void Item::lock(bool new_lock) { lock_ = new_lock; }
Sprite* Item::sprite() { return sprite_; }
Polygon* Item::click_area() { return &click_area_; }

void Item::accept(Visitor* visitor, TextManager* text_, PlayerManager* player_, RoomManager* room_, Mouse::click mouse) { visitor->visit(this, text_, player_, room_, mouse); }