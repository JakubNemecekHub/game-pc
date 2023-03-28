#include "../Visitor.hpp"

/*
    Include .hpp files here, because they are not included in the header.
*/
#include "../Item.hpp"
#include "../Door.hpp"
#include "../HotSpot.hpp"


void Visitor::visit_item(Item* item) {
    std::cout << item->get_observation() << "\n";
    }
void Visitor::visit_door(Door* door) {
    std::cout << door->locked() << "\n";
    }
void Visitor::visit_hot_spot(HotSpot* hot_spot) {
    std::cout << hot_spot->get_use_observation() << "\n";
    }