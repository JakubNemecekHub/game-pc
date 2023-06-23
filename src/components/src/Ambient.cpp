#include "../Ambient.hpp"

#include "../logic/State.hpp"
#include "../managers/RenderManager.hpp"


Ambient::Ambient(std::string id, Sprite* sprite, bool state)
    : GameObject{id, state}
{
    form_ = std::make_unique<Body>(sprite);
}


void Ambient::update(RenderManager* renderer, int dt)
{
    form_->update(renderer, dt);
}


void Ambient::accept_click(State* handler, Mouse::Status mouse) { handler->visit_click(this, mouse); }
void Ambient::accept_over(State* handler, Mouse::Status mouse) { handler->visit_over(this, mouse); }
void Ambient::accept_drag(State* handler, Mouse::Status mouse) { handler->visit_drag(this, mouse); }