#pragma once

#include "../utils/Mouse.hpp"


/*
    Use forward declaration.
    Including .hpp files would create circular dependency.
    The .hpp files are only included in Visitor.cpp
*/
class Item;
class Door;
class HotSpot;
class TextManager;
class PlayerManager;
class RoomManager;

class Visitor
{
public:
    void visit(Item* item, TextManager* text_, PlayerManager* player_, RoomManager* room_, Mouse::click mouse);
    void visit(Door* item, TextManager* text_, PlayerManager* player_, RoomManager* room_, Mouse::click mouse);
    void visit(HotSpot* item, TextManager* text_, PlayerManager* player_, RoomManager* room_, Mouse::click mouse);
};
