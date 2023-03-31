#include "../State.hpp"

#include "../../Game.hpp"
#include "../components/Sprite.hpp"

#define kkey event.key.keysym.sym // because i don't understand why this doesn't work: SDL_KeyCode key = event.key.keysym.sym;


/***********************************************************************************************************************
 * Intro
***********************************************************************************************************************/
IntroState IntroState::self_;
IntroState::IntroState() {}
IntroState* IntroState::get() { return &self_; }

bool IntroState::enter(Managers* managers)
{
    managers_ = managers;
    managers_->log.log("Entered Intro State.");
    logo_ = managers->assets.sprite("logo");
    logo_->scale_full_h();
    logo_->center();
    return true;
}

bool IntroState::exit()
{
    managers_->log.log("Left Intro State.");
    logo_ = nullptr;
    return true;
}

void IntroState::input(SDL_Event event)
{
    if ( event.type != SDL_KEYUP ) return;
    
    if ( event.key.keysym.sym == SDLK_RETURN )
    {
        managers_->state.next(Gameplay::Normal::get());
    }

}

void IntroState::update(int dt)
{
    logo_->update(&(managers_->renderer), dt);
}

void IntroState::render()
{
    managers_->renderer.render();
}

void IntroState::visit(Item* item, Mouse::click mouse_click_data) {}
void IntroState::visit(Door* door, Mouse::click mouse_click_data) {}
void IntroState::visit(HotSpot* hot_spot, Mouse::click mouse_click_data) {}


/***********************************************************************************************************************
 * Exit
***********************************************************************************************************************/
ExitState ExitState::self_;
ExitState::ExitState() {}
ExitState* ExitState::get() { return &self_; }

bool ExitState::enter(Managers* managers)
{
    managers_ = managers;
    managers_->log.log("Entered Exit State.");
    outro_ = managers->assets.sprite("outro");
    outro_->scale_full_h();
    outro_->center();
    return true;
}

bool ExitState::exit()
{
    managers_->log.log("Left Exit State.");
    outro_ = nullptr;
    return true;
}

void ExitState::input(SDL_Event event)
{
    if ( event.type != SDL_KEYUP ) return;
    
    if ( event.key.keysym.sym == SDLK_RETURN )
    {
        managers_->window.close();
    }

}

void ExitState::update(int dt)
{
    outro_->update(&(managers_->renderer), dt);
}

void ExitState::render()
{
    managers_->renderer.render();
}

void ExitState::visit(Item* item, Mouse::click mouse_click_data) {}
void ExitState::visit(Door* door, Mouse::click mouse_click_data) {}
void ExitState::visit(HotSpot* hot_spot, Mouse::click mouse_click_data) {}


/***********************************************************************************************************************
 * Gameplay Normal
***********************************************************************************************************************/
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

         if ( kkey == mapping.KEY_HOT_SPOTS ) managers_->rooms.handle_keyboard(ACTION_ROOM::BITMAP);            // Show click map
    else if ( kkey == mapping.KEY_WALK_POLYGON ) managers_->rooms.handle_keyboard(ACTION_ROOM::WALK_POLYGON);   // Toggle walk area's polygon rendering
    else if ( kkey == mapping.KEY_ITEM_POLYGON ) managers_->rooms.handle_keyboard(ACTION_ROOM::ITEM_POLYGON);   // Toggle items' click area's polygon rendering
    else if ( kkey == mapping.KEY_ITEM_VECTOR ) managers_->rooms.handle_keyboard(ACTION_ROOM::ITEM_VECTOR);     // Toggle items' position vector rendering
    else if ( kkey == mapping.KEY_INVENTORY ) managers_->state.next(Gameplay::Inventory::get());                // Show Inventory
}

void Gameplay::Normal::input_mouse_(SDL_Event event)
{
    if ( event.type != SDL_MOUSEBUTTONUP ) return;

    Mouse::click mouse_data { managers_->control.handle_mouse(event) }; // This could be merged with the next line.
    auto [x, y, right_click] = Mouse::destructure(mouse_data);          // Get mouse click information.
    // auto [x, y, right_click] = managers.control.mouse_data(event)    // Get mouse click information.

    GameObject* object { managers_->rooms.get_object(x, y) };           // Get object from Room Manager.
    if ( object) object->accept(this, mouse_data);                      // Do something with that object.
                                                                        // What do I need from the mouse data?
}

void Gameplay::Normal::input(SDL_Event event)
{
    input_keyboard_(event);
    input_mouse_(event);
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

void Gameplay::Normal::visit(Item* item, Mouse::click mouse)
{
    auto [x, y, right_click] = Mouse::destructure(mouse);
    if ( right_click )  // Look.
    {
        std::string observation { item->observation() };
        managers_->text.register_text(observation, x, y, COLOR::GREEN);
    }
    else                // Try to pick up. TO DO: move to Inventory
    {
        if ( managers_->player.inventory.has_space() )
        {
            managers_->player.inventory.add(item);
            managers_->rooms.remove_item(item->id());
            managers_->text.register_text(item->pick_observation(), x, y, COLOR::PURPLE);
        }
        else
        {
            managers_->text.register_text("My Inventory is full.", x, y, COLOR::BEIGE);
        }
    }
}


void Gameplay::Normal::visit(Door* door, Mouse::click mouse)
{
    auto [x, y, right_click] = Mouse::destructure(mouse);
    if ( right_click )  // Look
    {
        std::string observation { door->observation() };
        managers_->text.register_text(observation, x, y, COLOR::GREEN);
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
                managers_->text.register_text("Unlocked.", x, y, COLOR::PURPLE);
            }
            else
            {
                std::string observation { door->locked_observation() };
               managers_->text.register_text(observation, x, y, COLOR::GREEN);
            }
        }
        else
        {
            managers_->text.clean();
            managers_->rooms.activate_room(door->target());
        }
    }
}


void Gameplay::Normal::visit(HotSpot* hot_spot, Mouse::click mouse)
{
    auto [x, y, right_click] = Mouse::destructure(mouse);
    std::string observation;
    if ( right_click )
    {
        observation = hot_spot->observation();
        managers_->text.register_text(observation, x, y, COLOR::BEIGE);
    }
    else
    {
        observation = hot_spot->use_observation();
        managers_->text.register_text(observation, x, y, COLOR::GREEN);
    }
}


/***********************************************************************************************************************
 * Gameplay Inventory
***********************************************************************************************************************/
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