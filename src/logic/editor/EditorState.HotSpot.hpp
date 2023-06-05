#pragma once

#include "../State.hpp"

class GameObject;

class HotSpotEditor : public Gameplay::GameplayState
{
private:

    static HotSpotEditor self_;
    HotSpotEditor();

    Managers* managers_;

    // Helper

    void input_keyboard_(SDL_Event& event) override;
    void input_mouse_(SDL_Event& event) override;

    bool        mouse_down_;
    GameObject* selection_;

public:
    static HotSpotEditor* get();

    // State lifecycle

    bool enter(Managers* managers) override;
    bool exit() override;

    // Game loop methods

    void update(int dt) override; 
    void render() override;

    // Input methods

    void visit_over(HotSpot* hot_spot, SDL_Event& event) override;
    void visit_drag(HotSpot* hot_spot, SDL_Event& event) override;

};

