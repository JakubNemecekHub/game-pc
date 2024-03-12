#pragma once

#include <string>
#include <vector>

#include <SDL2/SDL.h>
extern "C" {
    #include "lua.h"
    #include "lualib.h"
    #include "lauxlib.h"
}
#include <sol/sol.hpp>

#include "../utils/Mouse.hpp"
#include "../math/Vector2D.hpp"

class Sprite;
class Managers;
class GameObject;
#include "../components/Item.hpp"
#include "../components/Door.hpp"
#include "../components/HotSpot.hpp"
#include "../components/Ambient.hpp"
#include "../components/Button.hpp"
#include "../components/Walkarea.hpp"

class State
{
private:

    Managers* managers_;
    sol::table script_;
    std::string name_;
    bool game_;
    int duration_;
    int timer_;

    GameObject* selection_;

    std::vector<Sprite*> sprites_;
    std::vector<Button>  buttons_;

    void load_sprite_(std::pair<sol::object, sol::object> sprite_data);
    void load_button_(std::pair<sol::object, sol::object> sprite);

    GameObject* get_join_object_(Vector2D position);

    void input_keyboard_(SDL_Event& event);
    void input_mouse_(SDL_Event& event);

public:

    // State lifecycle
    State();
    ~State() {};
    inline std::string name() { return name_; }
    inline void set(Managers* managers) { managers_ = managers; }
    bool enter(std::string state);
    bool exit();

    // Game loop methods

    void input(SDL_Event& event)
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
        // case SDL_MOUSEWHEEL:
            input_mouse_(event);
            break;
        }
    }
    void update(int dt);
    void render();

    inline void visit_click(Item* item, Mouse::Status mouse)         { script_["lua_visit_click_item"](item, mouse);         }
    inline void visit_click(Door* door, Mouse::Status mouse)         { script_["lua_visit_click_door"](door, mouse);         }
    inline void visit_click(HotSpot* hot_spot, Mouse::Status mouse)  { script_["lua_visit_click_hot_spot"](hot_spot, mouse); }
    inline void visit_click(Ambient* ambient, Mouse::Status mouse)   { script_["lua_visit_click_ambient"](ambient, mouse);   }
    inline void visit_click(Button* button, Mouse::Status mouse)     { button->action(); }
    inline void visit_click(WalkArea* walkarea, Mouse::Status mouse) { script_["lua_visit_click_walkarea"](walkarea, mouse); }

    inline void visit_over(Item* item, Mouse::Status mouse)         { script_["lua_visit_over_item"](item, mouse);          }
    inline void visit_over(Door* door, Mouse::Status mouse)         { script_["lua_visit_over_door"](door, mouse);          }
    inline void visit_over(HotSpot* hot_spot, Mouse::Status mouse)  { script_["lua_visit_over_hot_spot"](hot_spot, mouse);  }
    inline void visit_over(Ambient* ambient, Mouse::Status mouse)   { script_["lua_visit_over_ambient"](ambient, mouse);    }
    inline void visit_over(Button* button, Mouse::Status mouse)     { script_["lua_visit_over_button"](button, mouse);      }
    inline void visit_over(WalkArea* walkarea, Mouse::Status mouse) { script_["lua_visit_over_walkarea"](walkarea, mouse);  }

};
