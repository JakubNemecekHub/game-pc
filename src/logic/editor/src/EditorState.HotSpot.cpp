#include "../EditorState.HotSpot.hpp"

#include "../../../../Game.hpp"
#include "../EditorState.hpp"
#include "../EditorState.Item.hpp"
#include "../EditorState.WalkArea.hpp"
#include "../EditorState.Bitmap.hpp"
#include "../../Gameplay.NormalState.hpp"

#define kkey event.key.keysym.sym // because i don't understand why this doesn't work: SDL_KeyCode key = event.key.keysym.sym;


HotSpotEditor HotSpotEditor::self_;
HotSpotEditor::HotSpotEditor() {}
HotSpotEditor* HotSpotEditor::get() { return &self_; }


bool HotSpotEditor::enter(Managers* managers)
{
    managers_ = managers;
    managers_->text.submit_label("Editor: Hot Spot", 20, 30, COLOR::RED);
    managers_->rooms.handle_keyboard(ACTION_ROOM::EDITOR_HOT_SPOTS);
    return true;
}


bool HotSpotEditor::exit()
{
    managers_->text.clean();
    managers_->rooms.handle_keyboard(ACTION_ROOM::EDITOR_HOT_SPOTS);
    return true;
}


void HotSpotEditor::input_keyboard_(SDL_Event& event)
{
    if ( event.type != SDL_KEYUP ) return;
    const Controls mapping { managers_->control.mapping() };  // Mapping isn't constant, so we cannot use switch statement :( 
         if ( kkey == mapping.KEY_EDITOR )              managers_->state.next(Editor::get()); 
    else if ( kkey == mapping.KEY_EDITOR_ITEMS )        managers_->state.next(ItemEditor::get());    
    else if ( kkey == mapping.KEY_EDITOR_HOT_SPOTS )    managers_->state.next(Editor::get());   
    else if ( kkey == mapping.KEY_EDITOR_WALK_POLYGON ) managers_->state.next(WalkAreaEditor::get());  
    else if ( kkey == mapping.KEY_EDITOR_BITMAP )       managers_->state.next(BitmapEditor::get());    
    else if ( kkey == SDLK_F5 )                         managers_->items.save(&managers_->serial);
    else if ( kkey == SDLK_F6 )                         managers_->items.load(&managers_->serial);
}


void HotSpotEditor::input_mouse_(SDL_Event& event)
{
    auto[x, y] = managers_->control.mouse_position(event);

    GameObject* object { nullptr };
    object = managers_->rooms.get_hot_spot(x, y);

    switch (event.type)
    {
    case SDL_MOUSEMOTION:
        if ( mouse_down_ )
        {
            if ( selection_ ) selection_->accept_drag(this, event);
        }
        else
        {
            if ( object)
            {
                object->accept_over(this, event);
            }
            else managers_->text.clean_player();
        }
        break;
    case SDL_MOUSEBUTTONDOWN:
        selection_ = object;
        mouse_down_ = true;
        break;
    case SDL_MOUSEBUTTONUP:
        mouse_down_ = false;
        if ( object) object->accept_click(this, event);
        break;
    default:
        break;
    }
}


void HotSpotEditor::update(int dt)
{
    managers_->rooms.update(&(managers_->renderer), dt);
    managers_->player.update(&(managers_->renderer), dt);
    managers_->text.update(dt);
}


void HotSpotEditor::render()
{
    managers_->renderer.render();
}


/***********************************************************************************************************************************************************************
 * Visit Mouseover
************************************************************************************************************************************************************************/
void HotSpotEditor::visit_over(HotSpot* hot_spot, SDL_Event& event)
{
    auto[x, y] = managers_->control.mouse_position(event);
    std::string message { "ITEM: " + hot_spot->id() };
    managers_->text.submit_player(message, x, y, COLOR::RED);
}


/***********************************************************************************************************************************************************************
 * Visit Drag
************************************************************************************************************************************************************************/
void HotSpotEditor::visit_drag(HotSpot* hot_spot, SDL_Event& event)
{
    // This could only work for Hot spot with click polygon
}
