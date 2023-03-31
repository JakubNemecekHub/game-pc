#pragma once

#include "State.hpp"


namespace Gameplay
{

class Editor : public GameplayState
{
private:

    static Editor self_;
    Editor();

    Managers* managers_;

    // Helper

    void input_keyboard_(SDL_Event event);
    void input_mouse_(SDL_Event event);

public:
    static Editor* get();

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

}