#pragma once

#include "../State.hpp"

class GameObject;

class BitmapEditor : public Gameplay::GameplayState
{
private:

    static BitmapEditor self_;
    BitmapEditor();

    Managers* managers_;

    // Helper

    void input_keyboard_(SDL_Event& event) override;
    void input_mouse_(SDL_Event& event) override;

    bool        mouse_down_;
    GameObject* selection_;

public:
    static BitmapEditor* get();

    // State lifecycle

    bool enter(Managers* managers) override;
    bool exit() override;

    // Game loop methods

    void update(int dt) override; 
    void render() override;

    // Input methods

};

