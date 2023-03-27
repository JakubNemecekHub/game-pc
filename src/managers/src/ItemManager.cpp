#include "../ItemManager.hpp"

#include <queue>
#include <filesystem>

#include <yaml-cpp/yaml.h>

#include "../components/Item.hpp"
#include "../utils/Files.hpp"

namespace fs = std::filesystem;


ItemManager::ItemManager(LogManager* log)
    : log_{log} {}


bool ItemManager::startUp(AssetManager* assets)
{
    log_->log("Starting Item Manager.");
    std::queue<fs::directory_entry> items_meta { assets->items_meta() };
    while ( !items_meta.empty() )
    {
        fs::directory_entry entry { items_meta.front() };
        std::string id            { base_name(entry)   };
        YAML::Node item_data      { YAML::LoadFile(entry.path().string()) };
        item_data["id"] = id;
        items_.emplace(std::piecewise_construct,
                std::forward_as_tuple(id),
                std::forward_as_tuple(item_data, assets));
        log_->log("Item \"", id, "\" created.");
        items_meta.pop();
    }
    log_->log("Item Manager Started.");
    return true;
}


bool ItemManager::shutDown()
{
    // TO DO: Delete all Items
    log_->log("Shutting down Item Manager.");
    return true;
}


Item* ItemManager::get(std::string id)
{
    if ( items_.find(id) != items_.end() )
    {
        return &items_.at(id);
    }
    return nullptr;
}
