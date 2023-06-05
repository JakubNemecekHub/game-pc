#include "../EditorState.WalkArea.hpp"

#include "../../../../Game.hpp"
#include "../EditorState.hpp"
#include "../EditorState.Item.hpp"
#include "../EditorState.HotSpot.hpp"
#include "../EditorState.Bitmap.hpp"
#include "../../Gameplay.NormalState.hpp"

#define kkey event.key.keysym.sym // because i don't understand why this doesn't work: SDL_KeyCode key = event.key.keysym.sym;


WalkAreaEditor WalkAreaEditor::self_;
WalkAreaEditor::WalkAreaEditor() {}
WalkAreaEditor* WalkAreaEditor::get() { return &self_; }


bool WalkAreaEditor::enter(Managers* managers)
{
    managers_ = managers;
    managers_->text.submit_label("Editor: Walk Area", 20, 30, COLOR::RED);
    managers_->rooms.handle_keyboard(ACTION_ROOM::EDITOR_WALK_POLYGON);
    return true;
}


bool WalkAreaEditor::exit()
{
    managers_->text.clean();
    managers_->rooms.handle_keyboard(ACTION_ROOM::EDITOR_WALK_POLYGON);
    return true;
}


void WalkAreaEditor::input_keyboard_(SDL_Event& event)
{
    if ( event.type != SDL_KEYUP ) return;
    const Controls mapping { managers_->control.mapping() };  // Mapping isn't constant, so we cannot use switch statement :( 
         if ( kkey == mapping.KEY_EDITOR )              managers_->state.next(Editor::get());
    else if ( kkey == mapping.KEY_EDITOR_ITEMS )        managers_->state.next(ItemEditor::get());    
    else if ( kkey == mapping.KEY_EDITOR_HOT_SPOTS )    managers_->state.next(HotSpotEditor::get());   
    else if ( kkey == mapping.KEY_EDITOR_WALK_POLYGON ) managers_->state.next(Editor::get());  
    else if ( kkey == mapping.KEY_EDITOR_BITMAP )       managers_->state.next(BitmapEditor::get());    
    else if ( kkey == SDLK_F5 )                         managers_->items.save(&managers_->serial);
    else if ( kkey == SDLK_F6 )                         managers_->items.load(&managers_->serial);
}


void WalkAreaEditor::input_mouse_(SDL_Event& event)
{
    /*
        TO DO: Edit the position of pologon's vertices.
    auto[x, y] = managers_->control.mouse_position(event);

    switch (event.type)
    {
    case SDL_MOUSEMOTION:
        if ( mouse_down_ )
        {
        }
        else
        {

        }
        break;
    case SDL_MOUSEBUTTONDOWN:
        mouse_down_ = true;
        break;
    case SDL_MOUSEBUTTONUP:
        mouse_down_ = false;
        break;
    default:
        break;
    }
    */
}


void WalkAreaEditor::update(int dt)
{
    managers_->rooms.update(&(managers_->renderer), dt);
    managers_->player.update(&(managers_->renderer), dt);
    managers_->text.update(dt);
}


void WalkAreaEditor::render()
{
    managers_->renderer.render();
}
