#pragma once

#include "../State.hpp"

class GameObject;

class ItemEditor : public Gameplay::GameplayState
{
private:

    static ItemEditor self_;
    ItemEditor();

    Managers* managers_;

    // Helper

    void input_keyboard_(SDL_Event& event) override;
    void input_mouse_(SDL_Event& event) override;

    bool        mouse_down_;
    GameObject* selection_;

public:
    static ItemEditor* get();

    // State lifecycle

    bool enter(Managers* managers) override;
    bool exit() override;

    // Game loop methods

    void update(int dt) override; 
    void render() override;

    // Input methods

    void visit_over(Item* item, SDL_Event& event) override;
    void visit_drag(Item* item, SDL_Event& event) override;

};

