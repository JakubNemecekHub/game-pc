#include "../Gameplay.NormalState.hpp"

#include "../../Game.hpp"
#include "../components/Sprite.hpp"
#include "../Gameplay.InventoryState.hpp"
#include "../Gameplay.EditorState.hpp"

#define kkey event.key.keysym.sym // because i don't understand why this doesn't work: SDL_KeyCode key = event.key.keysym.sym;


Gameplay::Normal Gameplay::Normal::self_;
Gameplay::Normal::Normal() {}
Gameplay::Normal* Gameplay::Normal::get() { return &self_; }

bool Gameplay::Normal::enter(Managers* managers)
{
    // Load Room, items and such managers.
    // Right now, they are probably ready.
    managers_ = managers;
    return true;
}

bool Gameplay::Normal::exit()
{
    return true;
}

void Gameplay::Normal::input_keyboard_(SDL_Event event)
{
    if ( event.type != SDL_KEYUP ) return;

    const Controls mapping { managers_->control.mapping() };  // Mapping isn't constant, so we cannot use switch statement :( 
 
         if ( kkey == mapping.KEY_INVENTORY ) managers_->state.next(Gameplay::Inventory::get());                // Show Inventory
    else if ( kkey == mapping.KEY_EDITOR ) managers_->state.next(Gameplay::Editor::get());                      // Switch to editor
}

void Gameplay::Normal::input_mouse_(SDL_Event event)
{
    if ( event.type != SDL_MOUSEBUTTONUP ) return;

    Mouse::Transform mouse_transform { managers_->control.mouse_transform(event) };
    GameObject* object { managers_->rooms.get_object(mouse_transform.x, mouse_transform.y) };   // Get object from Room Manager.
    if ( object) object->accept_click(this, mouse_transform);                                         // Do something with that object.
}

void Gameplay::Normal::input(SDL_Event event)
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

void Gameplay::Normal::update(int dt)
{
    managers_->rooms.update(&(managers_->renderer), dt);
    managers_->player.update(&(managers_->renderer), dt);
    managers_->text.update(dt);
}

void Gameplay::Normal::render()
{
    managers_->renderer.render();
}

void Gameplay::Normal::visit_click(Item* item, Mouse::Transform mouse_transform)
{
    if ( mouse_transform.right_click )  // Look.
    {
        std::string observation { item->observation() };
        managers_->text.submit_player(observation, mouse_transform.x, mouse_transform.y, COLOR::GREEN);
    }
    else                // Try to pick up. TO DO: move to Inventory
    {
        if ( managers_->player.inventory.has_space() )
        {
            managers_->player.inventory.add(item);
            managers_->rooms.remove_item(item->id());
            managers_->text.submit_player(item->pick_observation(), mouse_transform.x, mouse_transform.y, COLOR::PURPLE);
        }
        else
        {
            managers_->text.submit_player("My Inventory is full.", mouse_transform.x, mouse_transform.y, COLOR::BEIGE);
        }
    }
}


void Gameplay::Normal::visit_click(Door* door, Mouse::Transform mouse_transform)
{
    if ( mouse_transform.right_click )  // Look
    {
        std::string observation { door->observation() };
        managers_->text.submit_player(observation, mouse_transform.x, mouse_transform.y, COLOR::GREEN);
    }
    else
    {
        if ( door->locked() )
        {
            std::string key_id { door->key_id() };
            if ( managers_->player.inventory.has_item(key_id) )
            {
                door->unlock();
                managers_->player.inventory.remove(key_id);
                managers_->text.submit_player("Unlocked.", mouse_transform.x, mouse_transform.y, COLOR::PURPLE);
            }
            else
            {
                std::string observation { door->locked_observation() };
               managers_->text.submit_player(observation, mouse_transform.x, mouse_transform.y, COLOR::GREEN);
            }
        }
        else
        {
            managers_->text.clean();
            managers_->rooms.activate_room(door->target());
        }
    }
}


void Gameplay::Normal::visit_click(HotSpot* hot_spot, Mouse::Transform mouse_transform)
{
    std::string observation;
    if ( mouse_transform.right_click )
    {
        observation = hot_spot->observation();
        managers_->text.submit_player(observation, mouse_transform.x, mouse_transform.y, COLOR::BEIGE);
    }
    else
    {
        observation = hot_spot->use_observation();
        managers_->text.submit_player(observation, mouse_transform.x, mouse_transform.y, COLOR::GREEN);
    }
}

void Gameplay::Normal::visit_over(Item* item, Mouse::Transform mouse_transform) {}
void Gameplay::Normal::visit_over(Door* door, Mouse::Transform mouse_transform) {}
void Gameplay::Normal::visit_over(HotSpot* hot_spot, Mouse::Transform mouse_transform) {}
