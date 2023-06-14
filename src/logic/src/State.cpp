#include "../State.hpp"

#include <string>
#include <algorithm>
#include <ranges>

#include "../../Game.hpp"   // Managers
#include "../components/Sprite.hpp"

State::State()
    : game_{false}, duration_{0}, timer_{0}, mouse_down_{false}, selection_{nullptr} {}


void State::load_sprite_(std::pair<sol::object, sol::object> sprite_data)
{
    sol::table data = sprite_data.second.as<sol::table>();
    std::string sprite_name = data.get<std::string>("sprite");
    sprites_.push_back(managers_->assets.sprite(sprite_name));
    Sprite* sprite = sprites_.back();
    bool fill = data.get_or("fill", false);
    if ( fill )
    {
        sprite->position(0, 0);
        sprite->scale_full_h();
        sprite->center();
    }
    else
    {
        float x = data.get<float>("x");
        float y = data.get<float>("y");
        float s = data.get<float>("scale");
        sprite->position(x, y);
        sprite->scale(s);
    }
}


void State::load_button_(std::pair<sol::object, sol::object> button_data)
{
    sol::table data = button_data.second.as<sol::table>();
    std::string text = data.get<std::string>("text");
    std::string sprite_name = data.get<std::string>("sprite");
    float x = data.get<float>("x");
    float y = data.get<float>("y");
    float s = data.get<float>("scale");
    sol::function action = data["fnc"];
    buttons_.emplace_back(text, managers_->assets.sprite(sprite_name), x, y, s, &managers_->text, action);
}


bool State::enter(std::string state)
{
    name_ = state;
    timer_ = 0;
    script_ = managers_->script.load_game_state(state);
    game_ = script_.get<bool>("game"); // This should always be in the script
    duration_ = script_.get_or("duration", 0);
    // Load sprites
    sol::table sprites = script_.get_or<sol::table>("sprites", sol::nil);   // This may not be in the script, must handle its absence
    if (sprites) std::ranges::for_each(sprites, [&](auto sprite) { load_sprite_(sprite); });
    // load the buttons
    sol::table buttons = script_.get_or<sol::table>("buttons", sol::nil);
    if (buttons) std::ranges::for_each(buttons, [&](auto button) { load_button_(button); });

    // Call state's lua enter function
    managers_->script.lua()->traverse_get<sol::function>("state", "enter")();

    return true;
}


void State::input_keyboard_(SDL_Event& event)
{
    // Need to convert the integer value of "event.key.keysym.sym" to some string from controls binging.
    // E.g. 105 = SDLK_i = "inventory"
    SDL_Keycode key_code = event.key.keysym.sym;
    // std::string key = managers_->control.key(key_code);
    std::string key = SDL_GetKeyName(key_code);
    managers_->script.lua()->traverse_get<sol::function>("state", "input_keyboard")(key);
}

GameObject* State::get_join_object_(float x, float y)
{
    for (auto& button : buttons_ ) if ( button.clicked(x, y) ) return &button;
    if (game_) return managers_->rooms.get_object(x, y);
    return nullptr;
}

void State::input_mouse_(SDL_Event& event)
{
    auto[x, y, r] = managers_->control.mouse_click(event);  // TO DO: use custom struct?
    GameObject* object { get_join_object_(x, y) };
    switch (event.type)
    {
    case SDL_MOUSEMOTION:
        if ( mouse_down_ )
        {
            // Left mouse button is pressed, we are draging an object
            // The state of the button could probably be determined derectly from the event
            if ( selection_ ) selection_->accept_drag(this, event);
        }
        else
        {
            if (object)
            {
                object->accept_over(this, x, y);
            }
            else managers_->text.clean_player();
        }
        break;
    case SDL_MOUSEBUTTONDOWN:
        selection_ = object;
        mouse_down_ = true;
        break;
    case SDL_MOUSEBUTTONUP:
        mouse_down_ = false;
        if (object) object->accept_click(this, x, y, r);
        break;
    default:
        break;
    }
}


bool State::exit()
{
    sprites_.clear(); // TO DO: this must "free" the sprites in AssetManager.
    buttons_.clear(); // TO DO:: This should destroy the buttons.

    // Call state's lua exit function
    managers_->script.lua()->traverse_get<sol::function>("state", "exit")();
    return true;
}


void State::update(int dt)
{

    timer_ += dt;
    if ( (duration_ != 0) && timer_ >= (duration_ * 1000) )
    {
        std::string next = script_.get<std::string>("next");
        managers_->state.next(next);
    }

    std::ranges::for_each(sprites_, [&](Sprite* sprite) { sprite->update(&(managers_->renderer), dt); });   // Update sprites
    std::ranges::for_each(buttons_, [&](Button& button) { button.update(&(managers_->renderer), dt); });    // Update buttons
    
    if ( !game_ ) return;
    // If we are in a gamestate, then update RoomManager, PlayerManager, and textManager
    managers_->rooms.update(&(managers_->renderer), dt);
    managers_->player.update(&(managers_->renderer), dt);
    managers_->text.update(dt);
}


void State::render()
{
    managers_->renderer.render();
}
