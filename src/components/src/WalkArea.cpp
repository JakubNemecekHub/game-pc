#include "../Walkarea.hpp"

#include "../logic/State.hpp"
#include "../../math/Polygon.hpp"


WalkArea::WalkArea(std::string id, Polygon polygon)
    : GameObject("wa_" + id, true)
{
    form_ = std::make_unique<Trigger>(polygon);
}

void WalkArea::update(RenderManager* renderer, int dt)
{
    form_->update(renderer, dt);
}

void WalkArea::accept_click(State* handler, Mouse::Status mouse) { handler->visit_click(this, mouse); }
void WalkArea::accept_over(State* handler, Mouse::Status mouse) { handler->visit_over(this, mouse); }
