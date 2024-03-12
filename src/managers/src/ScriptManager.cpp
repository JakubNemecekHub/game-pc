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
#include "../components/WalkArea.hpp"
#include "../utils/Mouse.hpp"
#include "../math/Polygon.hpp"
#include "../math/Vector2D.hpp"


ScriptManager::ScriptManager(LogManager* log)
    : log_{log} {}

/*
    Bind necessary C++ functions and member functions into "cpp" table in Lua state
*/
bool ScriptManager::startUp(std::string source_path, StateManager* state, TextManager* text, PlayerManager* player, RoomManager* rooms,
                            WindowManager* window, SerializationManager* io, ItemManager* items, AssetManager* assets)
{

    source_path_ = source_path + "/lgc/";

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
    lua_cpp_state.set_function("save", &RoomManager::save, rooms);

    // create "cpp.text" namespace
    auto lua_cpp_text  = lua_["cpp"]["text"].get_or_create<sol::table>();
    lua_cpp_text.set_function("player", &TextManager::submit_player, text);
    lua_cpp_text.set_function("label", &TextManager::submit_label, text);
    lua_cpp_text.set_function("free", &TextManager::submit_free, text);
    lua_cpp_text.set_function("clean", &TextManager::clean, text);
    lua_cpp_text.set_function("clean_label", sol::resolve<void()>(&TextManager::clean_label), text);
    lua_cpp_text.set_function("clean_specific_label", sol::resolve<void(std::string)>(&TextManager::clean_label), text);
    // COLOR enum
    lua_.new_enum("COLOR",
        "BEIGE",  COLOR::BEIGE,
        "PURPLE", COLOR::PURPLE,
        "GREEN",  COLOR::GREEN,
        "RED",    COLOR::RED
    );

    // create "cpp.player" namespace
    auto lua_cpp_player = lua_["cpp"]["player"].get_or_create<sol::table>();
    lua_cpp_player.set_function("walk", &Player::walk, &(player->player));
    lua_cpp_player.set_function("inventory_has_space", &Inventory::has_space, &(player->inventory));
    lua_cpp_player.set_function("inventory_add_item", &Inventory::add, &(player->inventory));
    lua_cpp_player.set_function("inventory_has_item", &Inventory::has_item, &(player->inventory));
    lua_cpp_player.set_function("inventory_remove_item", &Inventory::remove, &(player->inventory));
    lua_cpp_player.set_function("inventory_show", &Inventory::show, &(player->inventory));
    lua_cpp_player.set_function("inventory_hide", &Inventory::hide, &(player->inventory));

    // create "cpp.room" namespace
    auto lua_cpp_rooms = lua_["cpp"]["room"].get_or_create<sol::table>();
    lua_cpp_rooms.set_function("remove_item", &RoomManager::remove_item, rooms);
    lua_cpp_rooms.set_function("activate_room", &RoomManager::activate_room, rooms);

    lua_cpp_rooms.set_function("toggle_walk_area", &RoomManager::toggle_walk_area, rooms);
    lua_cpp_rooms.set_function("toggle_item_debug", &RoomManager::toggle_item_debug, rooms);
    lua_cpp_rooms.set_function("toggle_hot_spot_debug", &RoomManager::toggle_hot_spot_debug, rooms);
    lua_cpp_rooms.set_function("toggle_door_debug", &RoomManager::toggle_door_debug, rooms);


    // Item user type
    sol::usertype<Item> type_item = lua_.new_usertype<Item>("cpp_item");    // Defined without constructor, cannot create Item inside lua script
    // General GameObject stuff
    type_item["id"] = &Item::id;
    type_item["x"] = sol::property(sol::resolve<float()>(&Item::x), sol::resolve<void(float)>(&Item::x));
    type_item["y"] = sol::property(sol::resolve<float()>(&Item::y), sol::resolve<void(float)>(&Item::y));
    type_item["move"] = &Item::move;
    type_item["scale"] = sol::property(sol::resolve<float()>(&Item::scale), sol::resolve<void(float)>(&Item::scale));
    type_item["absolute_scale"] = &Item::set_scale;
    // Item Specific stuff
    type_item["observation"] = &Item::observation;
    type_item["pick_observation"] = &Item::pick_observation;

    // Door user type
    sol::usertype<Door> type_door = lua_.new_usertype<Door>("cpp_door");
    // General GameObject stuff
    type_door["id"] = &Door::id;
    type_door["x"] = sol::property(sol::resolve<float()>(&Door::x), sol::resolve<void(float)>(&Door::x));
    type_door["y"] = sol::property(sol::resolve<float()>(&Door::y), sol::resolve<void(float)>(&Door::y));
    type_door["move"] = &Item::move;
    type_door["scale"] = sol::property(sol::resolve<float()>(&Door::scale), sol::resolve<void(float)>(&Door::scale));
    type_door["absolute_scale"] = &Door::set_scale;
    // Door Specific stuff
    type_door["locked"] = &Door::locked;
    type_door["unlock"] = &Door::unlock;
    type_door["observation"] = &Door::observation;
    type_door["locked_observation"] = &Door::locked_observation;
    type_door["key_id"] = &Door::key_id;
    type_door["target"] = &Door::target;

    // Hot Spot user type
    sol::usertype<HotSpot> type_hot_spot = lua_.new_usertype<HotSpot>("cpp_hot_spot");
    // General GameObject stuff
    type_hot_spot["id"] = &HotSpot::id;
    type_hot_spot["x"] = sol::property(sol::resolve<float()>(&HotSpot::x), sol::resolve<void(float)>(&HotSpot::x));
    type_hot_spot["y"] = sol::property(sol::resolve<float()>(&HotSpot::y), sol::resolve<void(float)>(&HotSpot::y));
    type_hot_spot["move"] = &HotSpot::move;
    type_hot_spot["scale"] = sol::property(sol::resolve<float()>(&HotSpot::scale), sol::resolve<void(float)>(&HotSpot::scale));
    type_hot_spot["absolute_scale"] = &HotSpot::set_scale;
    // Hot Spot Specific stuff
    type_hot_spot["observation"] = &HotSpot::observation;
    type_hot_spot["use_observation"] = &HotSpot::use_observation;

    // Ambient user type
    sol::usertype<Ambient> type_ambient = lua_.new_usertype<Ambient>("cpp_ambient");
    type_ambient["id"] = &Ambient::id;

    // Button user type
    sol::usertype<Button> type_button = lua_.new_usertype<Button>("cpp_button");
    type_button["id"] = &Button::id;

    // WalkArea user type
    sol::usertype<WalkArea> type_walkarea = lua_.new_usertype<WalkArea>("cpp_walkarea");
    type_walkarea["get"] = &WalkArea::get;

    // Vector2D user type
    sol::usertype<Vector2D> type_vector2D = lua_.new_usertype<Vector2D>("cpp_vector2D", sol::constructors<Vector2D(float, float)>());
    type_vector2D["x"] = &Vector2D::x;
    type_vector2D["y"] = &Vector2D::y;
    type_vector2D["add"] = sol::resolve<Vector2D&(float, float)>(&Vector2D::add);

    // Mouse::BUTTON enumeration
    lua_.new_enum("BUTTON",
        "NONE", Mouse::BUTTON::NONE,
        "LEFT", Mouse::BUTTON::LEFT,
        "RIGHT", Mouse::BUTTON::RIGHT
    );
    // Mouse::Status user type
    sol::usertype<Mouse::Status> type_mouse = lua_.new_usertype<Mouse::Status>("cpp_mouse");
    type_mouse.set("position", sol::readonly(&Mouse::Status::position));
    type_mouse.set("relative", sol::readonly(&Mouse::Status::relative));
    type_mouse.set("b", sol::readonly(&Mouse::Status::b));

    // Polygon user type
    sol::usertype<Polygon> type_polygon = lua_.new_usertype<Polygon>("cpp_polygon");
    type_polygon["closest_vertex"] = &Polygon::closest_vertex;

    return true;
}


bool ScriptManager::shutdown() { return true; }


sol::table ScriptManager::load_game_state(std::string state_name)
{
    std::string file_path { source_path_ + state_name + ".lua" };
    lua_.script_file(file_path);
    return lua_["State"];
}
