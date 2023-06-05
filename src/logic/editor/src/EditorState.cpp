#include "../EditorState.hpp"

#include <format>

#include <SDL2/SDL.h>
#include <string>

#include "../../../../Game.hpp"
#include "../../../components/GameObject.hpp"
#include "../../Gameplay.NormalState.hpp"
#include "../EditorState.Item.hpp"
#include "../EditorState.HotSpot.hpp"
#include "../EditorState.WalkArea.hpp"
#include "../EditorState.Bitmap.hpp"

#define kkey event.key.keysym.sym // because i don't understand why this doesn't work: SDL_KeyCode key = event.key.keysym.sym;


Editor Editor::self_;
Editor::Editor() {}
Editor*Editor::get() { return &self_; }


bool Editor::enter(Managers* managers)
{
    managers_ = managers;
    managers_->text.submit_label("Editor", 20, 30, COLOR::RED);
    return true;
}


bool Editor::exit()
{
    managers_->text.clean();
    return true;
}


void Editor::input_keyboard_(SDL_Event& event)
{
    if ( event.type != SDL_KEYUP ) return;
    const Controls mapping { managers_->control.mapping() };  // Mapping isn't constant, so we cannot use switch statement :( 
         if ( kkey == mapping.KEY_EDITOR )              managers_->state.next(Gameplay::Normal::get());
    else if ( kkey == mapping.KEY_EDITOR_ITEMS )        managers_->state.next(ItemEditor::get());   
    else if ( kkey == mapping.KEY_EDITOR_HOT_SPOTS )    managers_->state.next(HotSpotEditor::get());   
    else if ( kkey == mapping.KEY_EDITOR_WALK_POLYGON ) managers_->state.next(WalkAreaEditor::get());  
    else if ( kkey == mapping.KEY_EDITOR_BITMAP )       managers_->state.next(BitmapEditor::get());    
    else if ( kkey == SDLK_F5 )                         managers_->items.save(&managers_->serial);
    else if ( kkey == SDLK_F6 )                         managers_->items.load(&managers_->serial);
}

// TO DO: Show object type on hover?
void Editor::input_mouse_(SDL_Event& event)
{
//     auto[x, y] = managers_->control.mouse_position(event);

//     GameObject* object { nullptr };
//     switch ( target_ )
//     {
//     case EDITOR_TARGET::ITEM:
//         object = managers_->rooms.get_item(x, y);
//         break;
//     case EDITOR_TARGET::HOT_SPOT:
//         object = managers_->rooms.get_hot_spot(x, y);
//         break;
//     default:
//         return;
//         break;
//     }
//     switch (event.type)
//     {
//     case SDL_MOUSEMOTION:
//         if ( mouse_down_ )
//         {
//             if ( selection_ ) selection_->accept_drag(this, event);
//         }
//         else
//         {
//             if ( object)
//             {
//                 object->accept_over(this, event);
//             }
//             else managers_->text.clean_player();
//         }
//         break;
//     case SDL_MOUSEBUTTONDOWN:
//         selection_ = object;
//         mouse_down_ = true;
//         break;
//     case SDL_MOUSEBUTTONUP:
//         mouse_down_ = false;
//         if ( object) object->accept_click(this, event);
//         break;
//     default:
//         break;
//     }
}


void Editor::update(int dt)
{
    managers_->rooms.update(&(managers_->renderer), dt);
    managers_->player.update(&(managers_->renderer), dt);
    managers_->text.update(dt);
}


void Editor::render()
{
    managers_->renderer.render();
}
