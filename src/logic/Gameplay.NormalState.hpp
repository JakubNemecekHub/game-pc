#pragma once

#include "State.hpp"


namespace Gameplay
{

class Normal : public GameplayState
{
private:

    static Normal self_;
    Normal();

    Managers* managers_;

    // Helper

    void input_keyboard_(SDL_Event& event) override;
    void input_mouse_(SDL_Event& event) override;

public:
    static Normal* get();

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

};

}