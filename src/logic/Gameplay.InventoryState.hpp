#pragma once

#include "State.hpp"


namespace Gameplay
{

class Inventory : public GameplayState
{
private:

    static Inventory self_;
    Inventory();

    Managers* managers_;

    // Helper

    void input_keyboard_(SDL_Event& event) override;
    void input_mouse_(SDL_Event& event) override;

public:
    static Inventory* get();

    // State lifecycle

    bool enter(Managers* managers) override;
    bool exit() override;

    // Game loop methods

    void update(int dt) override; 
    void render() override;

    // Input methods

    void visit_click(Item* item, SDL_Event& event) override;
    void visit_click(Door* door, SDL_Event& event) override;
    void visit_click(HotSpot* hot_spot, SDL_Event& event) override;
    void visit_click(Ambient* ambient, SDL_Event& event) override;
    void visit_over(Item* item, SDL_Event& event) override;
    void visit_over(Door* door, SDL_Event& event) override;
    void visit_over(HotSpot* hot_spot, SDL_Event& event) override;
    void visit_over(Ambient* ambient, SDL_Event& event) override;
    void visit_drag(Item* item, SDL_Event& event) override;
    void visit_drag(Door* door, SDL_Event& event) override;
    void visit_drag(HotSpot* hot_spot, SDL_Event& event) override;
    void visit_drag(Ambient* ambient, SDL_Event& event) override;

};

};