#include "../Ambient.hpp"

#include "../logic/States.hpp"


Ambient::Ambient(std::string id, Sprite* sprite, bool state)
    : GameObject{id, state}, sprite_{sprite} {}

std::string Ambient::id() { return id_; }
bool Ambient::state() { return state_; }
void Ambient::state(bool new_state) { state_ = new_state;}
Sprite* Ambient::sprite() { return sprite_; }



void Ambient::update(RenderManager* renderer, int dt)
{
    sprite_->update(renderer, dt);
}


bool Ambient::clicked(int x, int y)
{
    bool x_in { (sprite_->x() < x) && (x <= sprite_->x() + sprite_->w()) };
    bool y_in { (sprite_->y() < y) && (y <= sprite_->y() + sprite_->h()) };
    return x_in && y_in;
}


void Ambient::accept_click(Gameplay::GameplayState* handler, SDL_Event& event) { handler->visit_click(this, event); }
void Ambient::accept_over(Gameplay::GameplayState* handler, SDL_Event& event) { handler->visit_over(this, event); }
void Ambient::accept_drag(Gameplay::GameplayState* handler, SDL_Event& event) { handler->visit_drag(this, event); }