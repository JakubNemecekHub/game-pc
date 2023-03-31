#pragma once

#include <SDL2/SDL.h>

#include "../utils/Mouse.hpp"

class Managers;
class Sprite;
class Item;
class Door;
class HotSpot;


class State
{
public:

    // State lifecycle

    virtual bool enter(Managers* managers) = 0;
    virtual bool exit() = 0;
    ~State() {};

    // Game loop methods

    virtual void input(SDL_Event event) = 0; 
    virtual void update(int dt) = 0; 
    virtual void render() = 0; 

    // Input methods

    virtual void visit(Item* item, Mouse::click mouse_click_data) = 0;
    virtual void visit(Door* door, Mouse::click mouse_click_data) = 0;
    virtual void visit(HotSpot* hot_spot, Mouse::click mouse_click_data) = 0;
    
};


class IntroState : public State
{
private:

    static IntroState self_;
    IntroState();

    Managers* managers_;

    Sprite* logo_;

public:
    static IntroState* get();

    // State lifecycle

    bool enter(Managers* managers) override;
    bool exit() override;

    // Game loop methods

    void input(SDL_Event event) override; 
    void update(int dt) override; 
    void render() override;

    // Input methods TO DO: get rid of them in InventoryState. Only Gameplay states will use them.

    void visit(Item* item, Mouse::click mouse_click_data) override;
    void visit(Door* door, Mouse::click mouse_click_data) override;
    void visit(HotSpot* hot_spot, Mouse::click mouse_click_data) override;

};


class ExitState : public State
{
private:

    static ExitState self_;
    ExitState();

    Managers* managers_;

    Sprite* outro_;

public:
    static ExitState* get();

    // State lifecycle

    bool enter(Managers* managers) override;
    bool exit() override;

    // Game loop methods

    void input(SDL_Event event) override; 
    void update(int dt) override; 
    void render() override;

    // Input methods TO DO: get rid of them in InventoryState. Only Gameplay states will use them.

    void visit(Item* item, Mouse::click mouse_click_data) override;
    void visit(Door* door, Mouse::click mouse_click_data) override;
    void visit(HotSpot* hot_spot, Mouse::click mouse_click_data) override;

};


namespace Gameplay
{

class Normal : public State
{
private:

    static Normal self_;
    Normal();

    Managers* managers_;

    // Helper

    void input_keyboard_(SDL_Event event);
    void input_mouse_(SDL_Event event);

public:
    static Normal* get();

    // State lifecycle

    bool enter(Managers* managers) override;
    bool exit() override;

    // Game loop methods

    void input(SDL_Event event) override; 
    void update(int dt) override; 
    void render() override; 

    // Input methods

    void visit(Item* item, Mouse::click mouse_click_data) override;
    void visit(Door* door, Mouse::click mouse_click_data) override;
    void visit(HotSpot* hot_spot, Mouse::click mouse_click_data) override;

};


class Inventory : public State
{
private:

    static Inventory self_;
    Inventory();

    Managers* managers_;

    // Helper

    void input_keyboard_(SDL_Event event);
    void input_mouse_(SDL_Event event);

public:
    static Inventory* get();

    // State lifecycle

    bool enter(Managers* managers) override;
    bool exit() override;

    // Game loop methods

    void input(SDL_Event event) override; 
    void update(int dt) override; 
    void render() override;

    // Input methods

    void visit(Item* item, Mouse::click mouse_click_data) override;
    void visit(Door* door, Mouse::click mouse_click_data) override;
    void visit(HotSpot* hot_spot, Mouse::click mouse_click_data) override;

};

} // namespace