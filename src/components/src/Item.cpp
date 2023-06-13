#include "../Item.hpp"

#include <stdlib.h> // rand

#include "../logic/State.hpp"


Item::Item(YAML::Node data, AssetManager* assets)
    : GameObject{data["id"].as<std::string>(), false}
{
    lock_ = false;
    // Texts
    observations_ = data["observations"].as<std::vector<std::string>>();
    pick_observation_ = data["pick_observation"].as<std::string>();
    // Load click area polygon
    click_area_.add_vertices(data["click_area"].as<std::vector<std::vector<float>>>());
    // Load texture
    sprite_ = assets->sprite(id_);  // TO DO: log error if assets return nullptr
}

void Item::update(RenderManager* renderer, int dt)
{
    sprite_->update(renderer, dt);
    if ( debug_ )
    {
        renderer->submit(sprite_->position());
        renderer->submit(&click_area_);
        debug_ = false;
    }
}


bool Item::clicked(float x, float y)
{
    return click_area_.point_in_polygon(x, y) && state_;
}


std::string Item::observation()
{
    return observations_[rand() % observations_.size()];
}


std::string Item::pick_observation()
{
    return pick_observation_;
}


bool Item::lock()              { return lock_;        }
void Item::lock(bool new_lock) { lock_ = new_lock;    }
Sprite* Item::sprite()         { return sprite_;      }
Polygon* Item::click_area()    { return &click_area_; }

void Item::x(float x)
{
    float old_x { sprite_->x() };
    float dx { x - old_x };
    click_area_.move(dx, 0);
    sprite_->x(x);
}
void Item::y(float y)
{
    float old_y { sprite_->y() };
    float dy { y - old_y };
    click_area_.move(0, dy);
    sprite_->y(y);
}
void Item::move (float dx, float dy)
{
    sprite_->move(dx, dy);
    click_area_.move(dx, dy);
}

void Item::accept_click(State* handler, int x, int y, bool r) { handler->visit_click(this, x, y, r); }
void Item::accept_over(State* handler, SDL_Event& event) { handler->visit_over(this, event); }
void Item::accept_drag(State* handler, SDL_Event& event) { handler->visit_drag(this, event); }


/********* Serialization *********/

void Item::write(SerializationManager* io)
{
    io->write(lock_);
    sprite_->write(io);
}