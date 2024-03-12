#pragma once

#include <string>

#include "../utils/Sol.hpp"

class LogManager;
class StateManager;
class TextManager;
class PlayerManager;
class RoomManager;
class WindowManager;
class SerializationManager;
class ItemManager;
class AssetManager;
class RenderManager;


class ScriptManager
{
private:

    LogManager* log_;
    sol::state lua_;

    std::string source_path_;

public:

    ScriptManager() {};
    ScriptManager(LogManager* log);
    /*
        Starting up this manager will be tricky.
        All the C++ function needed in the lua scripts
        must be bound to lua state.
    */
    bool startUp(std::string source_path, StateManager* state, TextManager* text, PlayerManager* player, RoomManager* rooms, WindowManager* window, SerializationManager* io, ItemManager* items, AssetManager* assets, RenderManager* renderer);
    bool shutdown();


    inline sol::state* lua() { return &lua_; }
    // void load_script(std::string file_name);
    sol::table load_game_state(std::string file_name);

};
