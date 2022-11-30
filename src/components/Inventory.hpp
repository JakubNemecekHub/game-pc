#pragma once

#include <array>
#include <string>

#include "../managers/LogManager.hpp"
#include "Item.hpp"


constexpr int INVENTORY_SIZE { 7 };


class Inventory
{
private:

    std::array<Item*, INVENTORY_SIZE> items_ { nullptr };
    int count_;

public:

    Inventory();

    bool has_space();
    bool add(Item* item);
    bool has_item(std::string id);
    Item* get(std::string id);
    bool remove(std::string id);
    // bool remove(int index);
    // bool remove(Item* item);
    void print(LogManager* log);

};