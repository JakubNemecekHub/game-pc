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
    managers_->log.log("STOP");
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
    if ( event.type != SDL_MOUSEBUTTONUP ) return;

    Mouse::click mouse_data { managers_->control.handle_mouse(event) }; // This could be merged with the next line.
    auto [x, y, right_click] = Mouse::destructure(mouse_data);          // Get mouse click information.
    // auto [x, y, right_click] = managers.control.mouse_data(event)    // Get mouse click information.

    GameObject* object { managers_->rooms.get_object(x, y) };           // Get object from Room Manager.
    if ( object) object->accept(this, mouse_data);                      // Do something with that object.
                                                                        // What do I need from the mouse data?
}

void Gameplay::Editor::input(SDL_Event event)
{
    input_keyboard_(event);
    input_mouse_(event);
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

void Gameplay::Editor::visit(Item* item, Mouse::click mouse) {};
void Gameplay::Editor::visit(Door* door, Mouse::click mouse)
{
    auto [x, y, right_click] = Mouse::destructure(mouse);
    managers_->text.submit_player(door->target(), x, y, COLOR::RED);
};
void Gameplay::Editor::visit(HotSpot* hot_spot, Mouse::click mouse)
{
    
};