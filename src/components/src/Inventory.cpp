#include "../Inventory.hpp"


Inventory::Inventory()
{
    count_ = 0;
}


bool Inventory::has_space() {
    return count_ < INVENTORY_SIZE;
}


bool Inventory::add(Item* item)
{
    for ( auto& space : items_ )
    {
        if ( space == nullptr )
        {
            space = item;
            count_++;
            return true;
        }
    }
    return false;
}


bool Inventory::has_item(std::string id)
{
    for ( auto& space : items_ )
    {
        if ( space == nullptr ) continue;
        if ( space->id() == id )
        {
            return true;
        }
    }
    return false;
}

Item* Inventory::get(std::string id)
{
    for ( auto& space : items_ )
    {
        if ( space == nullptr ) continue;
        if ( space->id() == id )
        {
            return space;
        }
    }
    return nullptr;
}


bool Inventory::remove(std::string id)
{
    for ( auto& space : items_ )
    {
        if ( space == nullptr ) continue;
        if ( space->id() == id )
        {
            space = nullptr;
            return true;
        }
    }
    return false;
}


void Inventory::print(LogManager* log)
{
    std::string items { "Inventory: "};
    for ( auto& space : items_ )
    {
        if ( space != nullptr )
        {
            items += space->id();
            items += ", ";
        }
    }
    log->log(items);
}