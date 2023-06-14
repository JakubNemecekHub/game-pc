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

class Sprite;
class Managers;
class GameObject;
#include "../components/Item.hpp"
#include "../components/Door.hpp"
#include "../components/HotSpot.hpp"
#include "../components/Ambient.hpp"
#include "../components/Button.hpp"

class State
{
private:

    Managers* managers_;
    sol::table script_;
    std::string name_;
    bool game_;
    int duration_;
    int timer_;

    bool mouse_down_;
    GameObject* selection_;

    std::vector<Sprite*> sprites_;
    std::vector<Button>  buttons_;

    void load_sprite_(std::pair<sol::object, sol::object> sprite_data);
    void load_button_(std::pair<sol::object, sol::object> sprite);

    GameObject* get_join_object_(float x, float y);

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

    inline void visit_click(Item* item, int x, int y, bool r)        { script_["lua_visit_click_item"](item, x, y, r);         }
    inline void visit_click(Door* door, int x, int y, bool r)        { script_["lua_visit_click_door"](door, x, y, r);         }
    inline void visit_click(HotSpot* hot_spot, int x, int y, bool r) { script_["lua_visit_click_hot_spot"](hot_spot, x, y, r); }
    inline void visit_click(Ambient* ambient, int x, int y, bool r)  { script_["lua_visit_click_ambient"](ambient, x, y, r);   }
    inline void visit_click(Button* button, int x, int y, bool r)    { button->action(); }
    inline void visit_over(Item* item, int x, int y)         { script_["lua_visit_over_item"](item, x, y);          }
    inline void visit_over(Door* door, int x, int y)         { script_["lua_visit_over_door"](door, x, y);          }
    inline void visit_over(HotSpot* hot_spot, int x, int y)  { script_["lua_visit_over_hot_spot"](hot_spot, x, y);  }
    inline void visit_over(Ambient* ambient, int x, int y)   { script_["lua_visit_over_ambient"](ambient, x, y);    }
    inline void visit_over(Button* button, int x, int y)     { script_["lua_visit_over_button"](button, x, y); }
    inline void visit_drag(Item* item, SDL_Event& event)         { script_["lua_visit_drag_item"](item, event);          }
    inline void visit_drag(Door* door, SDL_Event& event)         { script_["lua_visit_drag_door"](door, event);          }
    inline void visit_drag(HotSpot* hot_spot, SDL_Event& event)  { script_["lua_visit_drag_hot_spot"](hot_spot, event);  }
    inline void visit_drag(Ambient* ambient, SDL_Event& event)   { script_["lua_visit_drag_ambient"](ambient, event);    }
    inline void visit_drag(Button* button, SDL_Event& event)     { script_["lua_visit_drag_button"](button, event);      }
    
};
