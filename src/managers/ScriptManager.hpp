#pragma once

#define SOL_ALL_SAFETIES_ON 1
#include <SDL2/SDL.h>
extern "C" {
    #include "lua.h"
    #include "lualib.h"
    #include "lauxlib.h"
}
#include <sol/sol.hpp>


class LogManager;
class StateManager;
class TextManager;
class PlayerManager;
class RoomManager;
class WindowManager;
class SerializationManager;
class ItemManager;
class AssetManager;

class ScriptManager
{
private:

    LogManager* log_;
    sol::state lua_;

public:

    ScriptManager() {};
    ScriptManager(LogManager* log);
    /*
        Starting up this manager will be tricky.
        All the C++ function needed in the lua scripts
        must be bound to lua state.
    */
    bool startUp(StateManager* state, TextManager* text, PlayerManager* player, RoomManager* rooms, WindowManager* window, SerializationManager* io, ItemManager* items, AssetManager* assets);
    bool shutdown();


    inline sol::state* lua() { return &lua_; } 
    // void load_script(std::string file_name);
    sol::table load_game_state(std::string file_name);

};