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

    void visit_click(Item* item, Mouse::Transform mouse_transform) override;
    void visit_click(Door* door, Mouse::Transform mouse_transform) override;
    void visit_click(HotSpot* hot_spot, Mouse::Transform mouse_transform) override;
    void visit_over(Item* item, Mouse::Transform mouse_transform) override;
    void visit_over(Door* door, Mouse::Transform mouse_transform) override;
    void visit_over(HotSpot* hot_spot, Mouse::Transform mouse_transform) override;

};

}