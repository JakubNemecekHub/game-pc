#include "../Gameplay.EditorState.hpp"

#include <SDL2/SDL.h>

#include "../../Game.hpp"
#include "../components/GameObject.hpp"
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
    auto[x, y] = managers_->control.mouse_position(event);
    GameObject* object { managers_->rooms.get_object(x, y) };   // Get object from Room Manager.
    switch (event.type)
    {
    case SDL_MOUSEMOTION:
        if ( mouse_down_ )
        {
            managers_->log.log("MOUSE DRAG", x, y);
            if ( selection_ ) selection_->accept_drag(this, event);
        }
        else
        {
            managers_->log.log("MOUSE MOVE", x, y);
            if ( object) object->accept_over(this, event);
            else managers_->text.clean_player();
        }
        break;
    case SDL_MOUSEBUTTONDOWN:
        selection_ = object;
        mouse_down_ = true;
        break;
    case SDL_MOUSEBUTTONUP:
        mouse_down_ = false;
        if ( object) object->accept_click(this, event);
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


/***********************************************************************************************************************************************************************
 * Visit Mouse click
************************************************************************************************************************************************************************/
void Gameplay::Editor::visit_click(Item* item, SDL_Event event)
{

}


void Gameplay::Editor::visit_click(Door* door, SDL_Event event)
{
    
}


void Gameplay::Editor::visit_click(HotSpot* hot_spot, SDL_Event event)
{
    
}

/***********************************************************************************************************************************************************************
 * Visit Mouseover
************************************************************************************************************************************************************************/
void Gameplay::Editor::visit_over(Item* item, SDL_Event event)
{
    auto[x, y] = managers_->control.mouse_position(event);
    managers_->text.submit_player(item->id(), x, y, COLOR::RED);
}


void Gameplay::Editor::visit_over(Door* door, SDL_Event event)
{
    auto[x, y] = managers_->control.mouse_position(event);
    managers_->text.submit_player(door->target(), x, y, COLOR::RED);
}


void Gameplay::Editor::visit_over(HotSpot* hot_spot, SDL_Event event)
{
    
}


/***********************************************************************************************************************************************************************
 * Visit Drag
************************************************************************************************************************************************************************/
void Gameplay::Editor::visit_drag(Item* item, SDL_Event event)
{
    int dx = event.motion.xrel;
    int dy = event.motion.yrel;
    item->move(dx, dy);
}


void Gameplay::Editor::visit_drag(Door* door, SDL_Event event)
{
    auto[x, y] = managers_->control.mouse_position(event);
    managers_->text.submit_player(door->target(), x, y, COLOR::RED);
}


void Gameplay::Editor::visit_drag(HotSpot* hot_spot, SDL_Event event)
{
    
}