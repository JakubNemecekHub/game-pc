#include "../ItemManager.hpp"

#include <queue>
#include <filesystem>

#include <yaml-cpp/yaml.h>

#include "LogManager.hpp"
#include "AssetManager.hpp"
#include "../components/Item.hpp"
#include "../utils/Files.hpp"

namespace fs = std::filesystem;


ItemManager::ItemManager(LogManager* log, AssetManager* assets, SerializationManager* io)
    : log_{log}, assets_{assets}, io_{io} {}


bool ItemManager::startUp()
{
    log_->log("Starting Item Manager.");
    std::queue<fs::directory_entry> items_meta { assets_->items_meta() };
    while ( !items_meta.empty() )
    {
        fs::directory_entry entry { items_meta.front() };
        try
        {
            std::string id       { base_name(entry)   };
            YAML::Node item_data { YAML::LoadFile(entry.path().string()) };
            item_data["id"] = id;
            items_.emplace(std::piecewise_construct,
                    std::forward_as_tuple(id),
                    std::forward_as_tuple(item_data, assets_));
            log_->log("Item \"", id, "\" created.");
        }
        catch(const YAML::ParserException& e)
        {
            log_->error("Cannot parse file", entry.path().string(), ", Reason: ", e.msg);
        }
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
    
void ItemManager::save()
{

    if ( !io_->open_out() ) return;

    io_->write( items_.size() );
    for ( auto& item : items_ )
    {
        io_->write(item.first);
        item.second.write(io_);
    }

    io_->close_out();

}


void ItemManager::load()
{

    // The very first value is the number of items in data.
    // item_id lock (sprite)  sprite_id x   y   scale  z current_depiction
    // bottle  1              bottle    514 480 24     0 bottle

    if ( !io_->open_in() ) return;

    int size; io_->read(size);
    for ( int i = 0; i < size; i++ )
    {
        // read in the data
        std::string item_id; io_->read(item_id);
        bool lock; io_->read(lock);
        std::string sprite_id; io_->read(sprite_id);
        float x; io_->read(x);
        float y; io_->read(y);
        float scale; io_->read(scale);
        int z_index; io_->read(z_index);
        std::string depiction; io_->read(depiction);
        // Update item
        Item* item { &items_.at(item_id) };
        item->set_use(lock);
        item->x(x);
        item->y(y);
        // Update item's sprite
        item->set_scale(scale);
        item->z_index(z_index);
        item->depiction(depiction);        
    }

    io_->close_in();

}