#include "../EditorState.Bitmap.hpp"

#include "../../../../Game.hpp"
#include "../EditorState.hpp"
#include "../EditorState.WalkArea.hpp"
#include "../EditorState.Item.hpp"
#include "../../Gameplay.NormalState.hpp"

#define kkey event.key.keysym.sym // because i don't understand why this doesn't work: SDL_KeyCode key = event.key.keysym.sym;


BitmapEditor BitmapEditor::self_;
BitmapEditor::BitmapEditor() {}
BitmapEditor* BitmapEditor::get() { return &self_; }


bool BitmapEditor::enter(Managers* managers)
{
    managers_ = managers;
    managers_->text.submit_label("Editor: Bitmap", 20, 30, COLOR::RED);
    managers_->rooms.handle_keyboard(ACTION_ROOM::EDITOR_BITMAP);
    return true;
}


bool BitmapEditor::exit()
{
    managers_->text.clean();
    managers_->rooms.handle_keyboard(ACTION_ROOM::EDITOR_BITMAP);
    return true;
}


void BitmapEditor::input_keyboard_(SDL_Event& event)
{
    if ( event.type != SDL_KEYUP ) return;
    const Controls mapping { managers_->control.mapping() };  // Mapping isn't constant, so we cannot use switch statement :( 
         if ( kkey == mapping.KEY_EDITOR )              managers_->state.next(Editor::get()); 
    else if ( kkey == mapping.KEY_EDITOR_ITEMS )        managers_->state.next(ItemEditor::get());    
    else if ( kkey == mapping.KEY_EDITOR_HOT_SPOTS )    managers_->state.next(Editor::get());   
    else if ( kkey == mapping.KEY_EDITOR_WALK_POLYGON ) managers_->state.next(WalkAreaEditor::get());  
    else if ( kkey == mapping.KEY_EDITOR_BITMAP )       managers_->state.next(Editor::get());    
    else if ( kkey == SDLK_F5 )                         managers_->items.save(&managers_->serial);
    else if ( kkey == SDLK_F6 )                         managers_->items.load(&managers_->serial);
}


void BitmapEditor::input_mouse_(SDL_Event& event) {}


void BitmapEditor::update(int dt)
{
    managers_->rooms.update(&(managers_->renderer), dt);
    managers_->player.update(&(managers_->renderer), dt);
    managers_->text.update(dt);
}


void BitmapEditor::render()
{
    managers_->renderer.render();
}
