#pragma once


/*
    Use forward declaration.
    Including .hpp files would create circular dependency.
    These .hpp are only included in Visitor.cpp
*/
class Item;
class Door;
class HotSpot;

class Visitor
{
public:
    Visitor() {}

    void visit_item(Item* item);
    void visit_door(Door* door);
    void visit_hot_spot(HotSpot* hot_spot);
};
