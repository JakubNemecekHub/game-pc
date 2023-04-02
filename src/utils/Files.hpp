#include <string>
#include <string_view>
#include <filesystem>

namespace fs = std::filesystem;


inline bool is_animation_meta(std::string_view name) { return name.find(".animation.") != std::string::npos; }
inline bool is_item_meta(std::string_view name)      { return name.find(".item.") != std::string::npos;      }
inline bool is_sprites_meta(std::string_view name)   { return name.find(".sprites.") != std::string::npos;   }
inline bool is_room_meta(std::string_view name)      { return name.find(".room.") != std::string::npos;      }
inline std::string base_name(fs::directory_entry entry)
{
    // D:\res\lamp.animation.yaml -> lamp
    // D:\res\lamp.item.yaml -> lamp
    std::string name { fs::path(entry).filename().string() };
    std::string::size_type length {name.find(".")};
    if (length == std::string::npos) return name;
    return name.substr(0, length);
}