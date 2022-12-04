#include "../ItemManager.hpp"

#include <yaml-cpp/yaml.h>

#include "../components/Item.hpp"


ItemManager::ItemManager(LogManager* log)
    : log_{log} {}


bool ItemManager::startUp(AssetManager* assets)
{
    log_->log("Starting Item Manager.");
    YAML::Node data = YAML::LoadFile(path_);
    for ( auto& item_data : data["items"] )
    {
        std::string id { item_data["id"].as<std::string>() };
        items_.emplace(std::piecewise_construct,
                std::forward_as_tuple(id),
                std::forward_as_tuple(item_data, assets));
    }
    log_->log("Item Manager Started.");
    return true;
}


bool ItemManager::shutDown()
{
    // Delete all Items
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
