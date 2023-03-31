#include "../Gameplay.InventoryState.hpp"

#include "../../Game.hpp"
#include "../components/Sprite.hpp"
#include "../Gameplay.NormalState.hpp"

#define kkey event.key.keysym.sym // because i don't understand why this doesn't work: SDL_KeyCode key = event.key.keysym.sym;


Gameplay::Inventory Gameplay::Inventory::self_;
Gameplay::Inventory::Inventory() {}
Gameplay::Inventory* Gameplay::Inventory::get() { return &self_; }

bool Gameplay::Inventory::enter(Managers* managers)
{
    managers_ = managers;
    managers_->player.inventory.show();
    return true;
}

bool Gameplay::Inventory::exit()
{
    managers_->player.inventory.hide();
    return true;
}

void Gameplay::Inventory::input_keyboard_(SDL_Event event)
{
    if ( event.type != SDL_KEYUP ) return;

    const Controls mapping { managers_->control.mapping() };  // Mapping isn't constant, so we cannot use switch statement :( 

    if ( kkey == mapping.KEY_INVENTORY ) managers_->state.next(Gameplay::Normal::get());
}

void Gameplay::Inventory::input_mouse_(SDL_Event event)
{
}

void Gameplay::Inventory::input(SDL_Event event)
{
    input_keyboard_(event);
    input_mouse_(event);
}

void Gameplay::Inventory::update(int dt)
{
    managers_->rooms.update(&(managers_->renderer), dt);
    managers_->player.update(&(managers_->renderer), dt);
    managers_->text.update(dt);
}

void Gameplay::Inventory::render()
{
    managers_->renderer.render();
}

void Gameplay::Inventory::visit(Item* item, Mouse::click mouse_click_data) {}
void Gameplay::Inventory::visit(Door* door, Mouse::click mouse_click_data) {}
void Gameplay::Inventory::visit(HotSpot* hot_spot, Mouse::click mouse_click_data) {}