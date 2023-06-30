#include "../Item.hpp"

#include <stdlib.h> // rand

#include "../logic/State.hpp"
#include "../managers/AssetManager.hpp"
#include "../managers/RenderManager.hpp"
#include "../managers/SerializationManager.hpp"
#include "../Sprite.hpp"


Item::Item(YAML::Node data, AssetManager* assets)
    : GameObject{data["id"].as<std::string>(), false}
{
    used_ = false;
    // Texts
    observations_ = data["observations"].as<std::vector<std::string>>();
    pick_observation_ = data["pick_observation"].as<std::string>();
    // Form, should be RigidBody
    // Others do not make much sense.
    form_ = std::make_unique<RigidBody>(assets->sprite(id_), data["click_area"].as<std::vector<std::vector<float>>>());
}


void Item::update(RenderManager* renderer, int dt)
{
    form_->update(renderer, dt);
}


std::string Item::observation()
{
    return observations_[rand() % observations_.size()];
}


std::string Item::pick_observation()
{
    return pick_observation_;
}


bool Item::is_used() { return used_; }
void Item::set_use(bool flag) { used_ = flag; }


void Item::accept_click(State* handler, Mouse::Status mouse) { handler->visit_click(this, mouse); }
void Item::accept_over(State* handler, Mouse::Status mouse) { handler->visit_over(this, mouse); }


/********* Serialization *********/

void Item::write(SerializationManager* io)
{
    io->write(used_);
    // sprite_->write(io); // TO DO: Serialize GameObject
}