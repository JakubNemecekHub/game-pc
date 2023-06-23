#include "../GameObject.hpp"

#include "../../managers/RenderManager.hpp"
#include "../../managers/TextManager.hpp"
#include "../Sprite.hpp"


/********************************************************************************
    BODY
********************************************************************************/
 void Body::x(float x) { sprite_->x(x); }
 void Body::y(float y) { sprite_->y(y); }
 void Body::position(float x, float y) { sprite_->x(x); sprite_->y(y); }
float Body::x() { return sprite_->x(); }
float Body::y() { return sprite_->y(); }
 void Body::move(float dx, float dy) { sprite_->move(dx, dy); }
 void Body::set_scale(float s) { sprite_->set_scale(s); }
 void Body::scale(float s) { sprite_->scale(s); }
float Body::scale() { return sprite_->scale(); }
 void Body::z_index(int z) { sprite_->z_index(z); }
  int Body::z_index() { return sprite_->z_index(); }
 void Body::depiction(std::string id) { sprite_->depiction(id); }

 void Body::update(RenderManager* renderer, int dt)
 {
    sprite_->update(renderer, dt);
    if ( debug )
    {
        renderer->submit(sprite_->position());
        debug = false;
    }
 }
 bool Body::clicked(float x, float y) { return false; } // TO DO: Check dest_rect for collision?


/********************************************************************************
    RIGID BODY
********************************************************************************/
RigidBody::RigidBody(Sprite* sprite, std::vector<std::vector<float>> vertices)
    : sprite_{sprite}
{
    click_area_.add_vertices(vertices);
}

 void RigidBody::x(float x)
 {
    float old_x { sprite_->x() };
    float dx { x - old_x };
    click_area_.move(dx, 0);
    sprite_->x(x);
 }
 void RigidBody::y(float y)
 {
    float old_y { sprite_->y() };
    float dy { y - old_y };
    click_area_.move(0, dy);
    sprite_->y(y);
 }
 void RigidBody::position(float x, float y) { this->x(x); this->y(y); }
float RigidBody::x() { return sprite_->x(); }
float RigidBody::y() { return sprite_->y(); }
 void RigidBody::move(float dx, float dy)
 {
    sprite_->move(dx, dy);
    click_area_.move(dx, dy);
 }
 void RigidBody::set_scale(float s)
 {
    float sp = s / sprite_->scale();  // polygon_scale_factor = final scale / original scale
    sprite_->set_scale(s);
    click_area_.scale(sp);
    click_area_.move((1.0f-sp)*sprite_->x(), (1.0f-sp)*sprite_->y());
 }
 void RigidBody::scale(float s)
 {
    sprite_->scale(s);
    click_area_.scale(s);
    click_area_.move((1.0f-s)*sprite_->x(), (1.0f-s)*sprite_->y()); // TO DO: implement vector arithmetic
 }
float RigidBody::scale() { return sprite_->scale(); }
 void RigidBody::z_index(int z) { sprite_->z_index(z); }
  int RigidBody::z_index() { return sprite_->z_index(); }
 void RigidBody::depiction(std::string id) { sprite_->depiction(id); }

 void RigidBody::update(RenderManager* renderer, int dt)
 {
    sprite_->update(renderer, dt);
    if ( debug )
    {
        renderer->submit(sprite_->position());
        renderer->submit(&click_area_);
        debug = false;
    }
 }
 bool RigidBody::clicked(float x, float y) { return click_area_.point_in_polygon(x, y); }


/********************************************************************************
    TRIGGER
********************************************************************************/
Trigger::Trigger(std::vector<std::vector<float>> vertices)
{
   click_area_.add_vertices(vertices);
}
// Polygon doesn't really have a "position"
// TO DO: implement envelope rectangle
void Trigger::x(float x) { click_area_.x(x); }
void Trigger::y(float y) { click_area_.y(y); }
void Trigger::position(float x, float y)
{
   click_area_.x(x);
   click_area_.y(y);
}
float Trigger::x() { return click_area_.x(); }
float Trigger::y() { return click_area_.y(); }
void Trigger::move(float dx, float dy) { click_area_.move(dx, dy); }
void Trigger::set_scale(float s)
{
   float sp = s / scale_;  // polygon_scale_factor = final scale / original scale
   scale_ = s;
   click_area_.scale(sp, true);
}
void Trigger::scale(float s)
{
   scale_ = s;
   click_area_.scale(s, true);
}
float Trigger::scale() { return scale_; }
void Trigger::z_index(int z) {}
int Trigger::z_index() { return 0; }
void Trigger::depiction(std::string id) {}

void Trigger::update(RenderManager* renderer, int dt)
{
   if ( debug )
   {
      renderer->submit(&click_area_);
      debug = false;
   }
}
bool Trigger::clicked(float x, float y) { return click_area_.point_in_polygon(x, y); }


/********************************************************************************
    BUTTON FORM
********************************************************************************/
ButtonForm::ButtonForm(Sprite* sprite, std::vector<std::vector<float>> vertices,
           TextManager* text, std::string label)
    : sprite_{sprite}, label_{label}
{
    click_area_.add_vertices(vertices);
    // Set up a textual label inside the Sprite.
   //  float x = sprite->x(); // This is not necessary
   //  float y = sprite->y();
   sprite_->match_dimensions(); // TO DO: Do it in the AssetManager
    label_sprite_ = std::make_unique<Sprite>(text->create_texture_(label_, COLOR::BEIGE), 1.0f, 3);
    label_sprite_->depiction("text");
    label_sprite_->match_dimensions();
    float label_scale = sprite_->w() / label_sprite_->w() * 0.8f;
    label_sprite_->set_scale(label_scale);
   //  float text_x { x + (sprite_->w() - label_sprite_->w())/2 }; // This doesn't work
   //  float text_y { y + (sprite_->h() - label_sprite_->h())/2 };
    float text_x { (sprite_->w() - label_sprite_->w())/2 };
    float text_y { (sprite_->h() - label_sprite_->h())/2 };
    label_sprite_->position(text_x, text_y);
}
void ButtonForm::x(float x)
{
   float old_x { sprite_->x() };
   float dx { x - old_x };
   click_area_.move(dx, 0);
   label_sprite_->move(dx, 0);
   sprite_->x(x);
}
void ButtonForm::y(float y)
{
   float old_y { sprite_->y() };
   float dy { y - old_y };
   click_area_.move(0, dy);
   label_sprite_->move(0, dy);
   sprite_->y(y);
}
void ButtonForm::position(float x, float y) { this->x(x); this->y(y); }
float ButtonForm::x() { return sprite_->x(); }
float ButtonForm::y() { return sprite_->y(); }
void ButtonForm::move(float dx, float dy)
{
   sprite_->move(dx, dy);
   label_sprite_->move(dx, dy);
   click_area_.move(dx, dy);
}
void ButtonForm::set_scale(float s)
{
   float sp = s / sprite_->scale();  // polygon_scale_factor = final scale / original scale
   sprite_->set_scale(s);
   label_sprite_->scale(s);
   click_area_.scale(sp);
   click_area_.move((1.0f-sp)*sprite_->x(), (1.0f-sp)*sprite_->y());
}
void ButtonForm::scale(float s)
{
   float dx = s*(label_sprite_->x() - sprite_->x());
   float dy = s*(label_sprite_->y() - sprite_->y());
   sprite_->scale(s);
   label_sprite_->scale(s);
    label_sprite_->move(dx, dy);
   click_area_.scale(s);
   click_area_.move((1.0f-s)*sprite_->x(), (1.0f-s)*sprite_->y());
}
float ButtonForm::scale() { return sprite_->scale(); }
void ButtonForm::z_index(int z) {
   sprite_->z_index(z);
   label_sprite_->z_index(z + 1); // TO DO: check what happens when this is out of z index bounds
}
int ButtonForm::z_index() { return sprite_->z_index(); }
void ButtonForm::depiction(std::string id) { sprite_->depiction(id); }

void ButtonForm::update(RenderManager* renderer, int dt)
{
   sprite_->update(renderer, dt);
   label_sprite_->update(renderer, dt);
   if ( debug )
   {
      renderer->submit(sprite_->position());
      renderer->submit(&click_area_);
      debug = false;
   }
}
bool ButtonForm::clicked(float x, float y) { return click_area_.point_in_polygon(x, y); }


/********************************************************************************
    GAME OBJECT
********************************************************************************/
GameObject::GameObject(std::string id, bool state)
    : id_{id}, state_{state} {}


// std::string GameObject::id() { return id_; }
// bool GameObject::state() { return state_; }
// void GameObject::state(bool new_state) { state_ = new_state; }


// DEBUG

// void GameObject::show_attributes() { debug_ = true; }