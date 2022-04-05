// TO DO: probably need to implement a copy construnctur for loading to work

#ifndef ANIMATION_COMPONENT_H
#define ANIMATION_COMPONENT_H

#include <iostream>
#include <unordered_map>
#include <string>
#include <vector>

#include <SDL2/SDL.h>

#include "../managers/RenderManager.hpp"
#include "../components/Texture.hpp"


class Frame
{
public:
    SDL_Rect src_rect;
    int duration;
};

/*
    Potřebuji constructor, kterému dám pointer na hotový objekt Texture
        tu získám z RenderManager::GetInstance()->load_texture
        (Object asi musí být vytvořen mimo tento objek, jinak by hned vypadl ze scope.
        Tak to není!
         Vypadá to ale, že to tak máme)
    Pak také hotové frames
        ty získám během Animation::load_animation
    Offset pro x i y
        ty získám během Animation::load_animation
    Vývhozí hodnoty
        current_frame je 0
        last_updated 0

    Pole bych si měl schovat pod private

    Animace pro hráče chci mít v unordered_map, abych podle akce mohl nastavit konkrétní animaci.
    Např. "walk", "idle", "pick_up".
    Ambient animace místnosti by naopak bylo lepší mít uložené ve vectoru či std::array. Při každém update
    je musím prosít a aktualizovat všechny.

    TO DO in distant future:
    přidat pole speed: abych mohl měnit rychlost přehrávání animace (něco jako frame.duration * speed)
*/
class Animation
{
private:
public:
    // Fields
    Texture* texture;
    int current_frame;  //this is src_rect. dest_rect and scale will be provided by RoomManager.
    long last_updated;
    std::vector<Frame> frames;
    int offset_x; // How is this used?
    int offset_y; // How is this used?

    // Constructor
    Animation() {};
    Animation(Texture* _texture,
              std::vector<Frame> _frames,
              int _offset_x = 0, int _offset_y = 0,
              int _x = 0, int _y = 0
            );
    // Desctuctor
    ~Animation();
    // Static Class Methods
    static std::unordered_map<std::string, Animation> load_animation(std::string file_name);
    // Methods
    void reset();
    void update(int dt);
    void set_position(int _x, int _y);
};

#endif // ANIMATION_COMPONENT_H