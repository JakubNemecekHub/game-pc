#include "../Gameplay.EditorState.hpp"

#include "../../Game.hpp"
#include "../Gameplay.NormalState.hpp"

#define kkey event.key.keysym.sym // because i don't understand why this doesn't work: SDL_KeyCode key = event.key.keysym.sym;


Gameplay::Editor Gameplay::Editor::self_;
Gameplay::Editor::Editor() {}
Gameplay::Editor* Gameplay::Editor::get() { return &self_; }

bool Gameplay::Editor::enter(Managers* managers)
{
    // Load Room, items and such managers.
    // Right now, they are probably ready.
    managers_ = managers;
    managers_->text.submit_label("Editor", 20, 30, COLOR::RED);
    return true;
}

bool Gameplay::Editor::exit()
{
    managers_->text.clean();
    return true;
}

void Gameplay::Editor::input_keyboard_(SDL_Event event)
{
    if ( event.type != SDL_KEYUP ) return;

    const Controls mapping { managers_->control.mapping() };  // Mapping isn't constant, so we cannot use switch statement :( 

         if ( kkey == mapping.KEY_EDITOR ) managers_->state.next(Gameplay::Normal::get());                      // Return to normal state
    else if ( kkey == mapping.KEY_HOT_SPOTS ) managers_->rooms.handle_keyboard(ACTION_ROOM::BITMAP);            // Show click map
    else if ( kkey == mapping.KEY_WALK_POLYGON ) managers_->rooms.handle_keyboard(ACTION_ROOM::WALK_POLYGON);   // Toggle walk area's polygon rendering
    else if ( kkey == mapping.KEY_ITEM_POLYGON ) managers_->rooms.handle_keyboard(ACTION_ROOM::ITEM_POLYGON);   // Toggle items' click area's polygon rendering
    else if ( kkey == mapping.KEY_ITEM_VECTOR ) managers_->rooms.handle_keyboard(ACTION_ROOM::ITEM_VECTOR);     // Toggle items' position vector rendering
}

void Gameplay::Editor::input_mouse_(SDL_Event event)
{
    Mouse::Transform mouse_transform { managers_->control.mouse_transform(event) };             // Get mouse information.
    GameObject* object { managers_->rooms.get_object(mouse_transform.x, mouse_transform.y) };   // Get object from Room Manager.
    switch (event.type)
    {
    case SDL_MOUSEBUTTONUP:
        if ( object) object->accept_click(this, mouse_transform);
        break;
    case SDL_MOUSEMOTION:
            if ( object) object->accept_over(this, mouse_transform);
            else managers_->text.clean_player();
        break;
    default:
        break;
    }
}

void Gameplay::Editor::input(SDL_Event event)
{
    switch (event.type)
    {
    // case SDL_KEYDOWN:
    // case SDL_TEXTEDITING:
    // case SDL_TEXTINPUT:
    case SDL_KEYUP:
        input_keyboard_(event);
        break;
    case SDL_MOUSEMOTION:
    case SDL_MOUSEBUTTONDOWN:
    case SDL_MOUSEBUTTONUP:
    case SDL_MOUSEWHEEL:
        input_mouse_(event);
        break;
    }
}

void Gameplay::Editor::update(int dt)
{
    managers_->rooms.update(&(managers_->renderer), dt);
    managers_->player.update(&(managers_->renderer), dt);
    managers_->text.update(dt);
}

void Gameplay::Editor::render()
{
    managers_->renderer.render();
}

void Gameplay::Editor::visit_click(Item* item, Mouse::Transform mouse_transform)
{

}
void Gameplay::Editor::visit_click(Door* door, Mouse::Transform mouse_transform)
{
    
}
void Gameplay::Editor::visit_click(HotSpot* hot_spot, Mouse::Transform mouse_transform)
{
    
}


void Gameplay::Editor::visit_over(Item* item, Mouse::Transform mouse_transform)
{
    managers_->text.submit_player(item->id(), mouse_transform.x, mouse_transform.y, COLOR::RED);
}
void Gameplay::Editor::visit_over(Door* door, Mouse::Transform mouse_transform)
{
    managers_->text.submit_player(door->target(), mouse_transform.x, mouse_transform.y, COLOR::RED);
}
void Gameplay::Editor::visit_over(HotSpot* hot_spot, Mouse::Transform mouse_transform)
{
    
}
