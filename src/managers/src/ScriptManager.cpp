#include "../ScriptManager.hpp"

#include "LogManager.hpp"
#include "StateManager.hpp"
#include "TextManager.hpp"
#include "PlayerManager.hpp"
#include "RoomManager.hpp"
#include "RoomManager.hpp"
#include "WindowManager.hpp"
#include "../logic/State.hpp"
#include "../components/Item.hpp"
#include "../components/Door.hpp"
#include "../components/HotSpot.hpp"
#include "../components/Inventory.hpp"


ScriptManager::ScriptManager(LogManager* log)
    : log_{log} {}

/*
    Bind necessary C++ functions and member functions into "cpp" table in Lua state
*/
bool ScriptManager::startUp(StateManager* state, TextManager* text, PlayerManager* player, RoomManager* rooms,
                            WindowManager* window, SerializationManager* io, ItemManager* items, AssetManager* assets)
{
    lua_.open_libraries(sol::lib::base, sol::lib::package); // TO DO: Check what libraries we need
    // Set up managers' member functions
    // create "cpp" namespace
    auto lua_cpp = lua_["cpp"].get_or_create<sol::table>();

    // create "cpp.game" namespace
    auto lua_cpp_game = lua_["cpp"]["game"].get_or_create<sol::table>();  
    lua_cpp_game.set_function("start_manager_item", &ItemManager::startUp, items);
    lua_cpp_game.set_function("start_manager_room", &RoomManager::startUp, rooms);
    lua_cpp_game.set_function("start_manager_player", &PlayerManager::startUp, player);

    // create "cpp.window" namespace
    auto lua_cpp_window = lua_["cpp"]["window"].get_or_create<sol::table>();  
    lua_cpp_window.set_function("close", &WindowManager::close, window);

    // create "cpp.state" namespace
    auto lua_cpp_state = lua_["cpp"]["state"].get_or_create<sol::table>();  
    lua_cpp_state.set_function("next", &StateManager::next, state);

    // create "cpp.text" namespace
    auto lua_cpp_text  = lua_["cpp"]["text"].get_or_create<sol::table>();   
    lua_cpp_text.set_function("player", &TextManager::submit_player, text);
    lua_cpp_text.set_function("label", &TextManager::submit_label, text);
    lua_cpp_text.set_function("free", &TextManager::submit_free, text);
    lua_cpp_text.set_function("clean", &TextManager::clean, text);

    // create "cpp.player" namespace
    auto lua_cpp_player = lua_["cpp"]["player"].get_or_create<sol::table>();    
    lua_cpp_player.set_function("inventory_has_space", &Inventory::has_space, &(player->inventory));
    lua_cpp_player.set_function("inventory_add_item", &Inventory::add, &(player->inventory));
    lua_cpp_player.set_function("inventory_has_item", &Inventory::has_item, &(player->inventory));
    lua_cpp_player.set_function("inventory_remove_item", &Inventory::remove, &(player->inventory));

    // create "cpp.room" namespace
    auto lua_cpp_rooms = lua_["cpp"]["room"].get_or_create<sol::table>();    
    lua_cpp_rooms.set_function("remove_item", &RoomManager::remove_item, rooms);
    lua_cpp_rooms.set_function("activate_room", &RoomManager::activate_room, rooms);
    lua_cpp_rooms.set_function("load_items", &ItemManager::load, items);


    // Item user type
    sol::usertype<Item> type_item = lua_.new_usertype<Item>("cpp_item");    // Defined without constructor, cannot create Item inside lua script
    type_item["observation"] = &Item::observation;
    type_item["pick_observation"] = &Item::pick_observation;
    type_item["id"] = &Item::id;

    // Door user type
    sol::usertype<Door> type_door = lua_.new_usertype<Door>("cpp_door");
    type_door["locked"] = &Door::locked;
    type_door["unlock"] = &Door::unlock;
    type_door["observation"] = &Door::observation;
    type_door["locked_observation"] = &Door::locked_observation;
    type_door["key_id"] = &Door::key_id;
    type_door["target"] = &Door::target;

    // Hot Spot user type
    sol::usertype<HotSpot> type_hot_spot = lua_.new_usertype<HotSpot>("cpp_hot_spot");
    type_hot_spot["observation"] = &HotSpot::observation;
    type_hot_spot["use_observation"] = &HotSpot::use_observation;

    return true;
}


bool ScriptManager::shutdown() { return true; }


sol::table ScriptManager::load_game_state(std::string file_name)
{
    lua_.script_file(file_name);
    return lua_["state"];
}