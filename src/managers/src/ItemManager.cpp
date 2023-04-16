#include "../ItemManager.hpp"

#include <queue>
#include <filesystem>

#include <yaml-cpp/yaml.h>

#include "LogManager.hpp"
#include "AssetManager.hpp"
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


// Serialization
    
void ItemManager::save(SerializationManager* io)
{

    if ( !io->open_out() ) return;

    io->write( items_.size() );
    for ( auto& item : items_ )
    {
        io->write(item.first);
        item.second.write(io);
    }

    io->close_out();

}


void ItemManager::load(SerializationManager* io)
{

    // The very first value is the number of items in data.
    // item_id lock (sprite)  sprite_id x   y   scale  z current_depiction
    // bottle  1              bottle    514 480 24     0 bottle

    if ( !io->open_in() ) return;

    int size; io->read(size);
    for ( int i = 0; i < size; i++ )
    {
        // read in the data
        std::string item_id; io->read(item_id);
        bool lock; io->read(lock);
        std::string sprite_id; io->read(sprite_id);
        int x; io->read(x);
        int y; io->read(y);
        float scale; io->read(scale);
        int z_index; io->read(z_index);
        std::string depiction; io->read(depiction);
        // Update item
        Item* item { &items_.at(item_id) };
        item->lock(lock);
        item->x(x);
        item->y(y);
        // Update item's sprite
        item->sprite()->scale(scale);
        item->sprite()->z_index(z_index);
        item->sprite()->depiction(depiction);
    }

    io->close_in();

}