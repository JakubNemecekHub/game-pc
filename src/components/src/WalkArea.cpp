#include "../Walkarea.hpp"

#include "../logic/State.hpp"


WalkArea::WalkArea(std::string id, std::vector<std::vector<float>> vertices)
    : GameObject("wa_" + id, true)
{
    form_ = std::make_unique<Trigger>(vertices);
}
WalkArea::WalkArea(std::string id, std::vector<std::vector<float>> vertices, float room_x, float room_y, float room_scale)
    : WalkArea(id, vertices)
{
    form_->scale(room_scale);
    form_->move(room_x, room_y);
}

void WalkArea::update(RenderManager* renderer, int dt)
{
    form_->update(renderer, dt);
}

void WalkArea::accept_click(State* handler, Mouse::Status mouse) { handler->visit_click(this, mouse); }
void WalkArea::accept_over(State* handler, Mouse::Status mouse) { handler->visit_over(this, mouse); }