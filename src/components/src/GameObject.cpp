#include "../GameObject.hpp"

#include "../../managers/RenderManager.hpp"
#include "../../managers/TextManager.hpp"
#include "../../managers/SerializationManager.hpp"
#include "../Sprite.hpp"


/********************************************************************************
    BODY
********************************************************************************/
 void Body::x(float x) { sprite_->x(x); }
 void Body::y(float y) { sprite_->y(y); }
 void Body::position(Vector2D position) { sprite_->position(position); }
float Body::x() { return sprite_->x(); }
float Body::y() { return sprite_->y(); }
 void Body::move(Vector2D direction) { sprite_->move(direction); }
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
bool Body::clicked(Vector2D position) { return false; } // TO DO: Check dest_rect for collision?

void Body::write(SerializationManager* io)
{
   io->write("BODY");
   sprite_->write(io);
}


/********************************************************************************
    RIGID BODY
********************************************************************************/
RigidBody::RigidBody(Sprite* sprite, Polygon trigger)
    : sprite_{sprite}, click_area_{trigger} {}

void RigidBody::x(float x)
{
   Vector2D direction {x - sprite_->x(), 0};
   sprite_->move(direction);
   click_area_.move(direction);
}
void RigidBody::y(float y)
{
   Vector2D direction {0, y - sprite_->y()};
   sprite_->move(direction);
   click_area_.move(direction);
}
void RigidBody::position(Vector2D position)
{
   this->x(position.x);
   this->y(position.y);
   // TO DO: Identify error in the code below
   // sprite_->position(position);
   // click_area_.position(position);
}
float RigidBody::x() { return sprite_->x(); }
float RigidBody::y() { return sprite_->y(); }
void RigidBody::move(Vector2D direction)
{
   sprite_->move(direction);
   click_area_.move(direction);
}
void RigidBody::set_scale(float s)
{
   float sp = s / sprite_->scale();  // polygon_scale_factor = final scale / original scale
   sprite_->set_scale(s);
   click_area_.scale(sp);
   click_area_.move(*(sprite_->position()) * (1.0f - s));
}
void RigidBody::scale(float s)
{
   sprite_->scale(s);
   click_area_.scale(s);
   click_area_.move(*(sprite_->position()) * (1.0f - s));
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
 bool RigidBody::clicked(Vector2D position) { return click_area_.point_in_polygon(position); }

void RigidBody::write(SerializationManager* io)
{
   io->write("RIGIDBODY");
   sprite_->write(io);
   std::ranges::for_each(click_area_.vertices, [&](const Vector2D& v)
   {
      io->write(v.x);
      io->write(v.y);
   });
}

/********************************************************************************
    TRIGGER
********************************************************************************/
Trigger::Trigger(Polygon trigger)
   : click_area_{trigger} {}

void Trigger::x(float x) { click_area_.x(x); }
void Trigger::y(float y) { click_area_.y(y); }
void Trigger::position(Vector2D position) { click_area_.position(position); }
float Trigger::x() { return click_area_.x(); }
float Trigger::y() { return click_area_.y(); }
void Trigger::move(Vector2D direction) { click_area_.move(direction); }
void Trigger::set_scale(float s)
{
   float sp = s / scale_;  // polygon_scale_factor = final scale / original scale
   scale_ = s;
   click_area_.scale(sp, true);
}
void Trigger::scale(float s)
{
   scale_ *= s;
   click_area_.scale(s, true);
}
float Trigger::scale() { return scale_; }
void Trigger::z_index(int z) {}
int Trigger::z_index() { return 0; }
void Trigger::depiction(std::string id) {}

Polygon* Trigger::get() { return &click_area_; }

void Trigger::update(RenderManager* renderer, int dt)
{
   if ( debug )
   {
      renderer->submit(&click_area_);
      debug = false;
   }
}
bool Trigger::clicked(Vector2D position) { return click_area_.point_in_polygon(position); }

void Trigger::write(SerializationManager* io)
{
   io->write("TRIGGER");
   io->write(scale_);
   std::ranges::for_each(click_area_.vertices, [&](const Vector2D& v)
   {
      io->write(v.x);
      io->write(v.y);
   });
}

/********************************************************************************
    BUTTON FORM
********************************************************************************/
ButtonForm::ButtonForm(Sprite* sprite, Polygon trigger, TextManager* text, std::string label)
    : sprite_{sprite}, click_area_{trigger}, label_{label}
{
    // Set up a textual label inside the Sprite.
   sprite_->match_dimensions(); // TO DO: Do it in the AssetManager
   label_sprite_ = std::make_unique<Sprite>(text->create_texture_(label_, COLOR::BEIGE), 1.0f, 3);
   label_sprite_->depiction("text");
   label_sprite_->match_dimensions();
   float label_scale = sprite_->w() / label_sprite_->w() * 0.8f;
   label_sprite_->set_scale(label_scale);
   Vector2D text_position { (sprite_->w() - label_sprite_->w())/2,
                            (sprite_->h() - label_sprite_->h())/2  };
   label_sprite_->position(text_position);
}
void ButtonForm::x(float x)
{
   float old_x { sprite_->x() };
   float dx { x - old_x };
   click_area_.move(Vector2D{dx, 0});
   label_sprite_->move(Vector2D{dx, 0});
   sprite_->x(x);
}
void ButtonForm::y(float y)
{
   float old_y { sprite_->y() };
   float dy { y - old_y };
   click_area_.move(Vector2D{0, dy});
   label_sprite_->move(Vector2D{0, dy});
   sprite_->y(y);
}
void ButtonForm::position(Vector2D position)
{
   this->x(position.x);
   this->y(position.y);
}
float ButtonForm::x() { return sprite_->x(); }
float ButtonForm::y() { return sprite_->y(); }
void ButtonForm::move(Vector2D direction)
{
   sprite_->move(direction);
   label_sprite_->move(direction);
   click_area_.move(direction);
}
void ButtonForm::set_scale(float s)
{
   float sp = s / sprite_->scale();  // polygon_scale_factor = final scale / original scale
   sprite_->set_scale(s);
   label_sprite_->scale(s);
   click_area_.scale(sp);
   click_area_.move(*(sprite_->position()) * (1.0f - s));
}
void ButtonForm::scale(float s)
{
   Vector2D direction = (*(label_sprite_->position()) - *(sprite_->position())) * s;
   sprite_->scale(s);
   label_sprite_->scale(s);
    label_sprite_->move(direction);
   click_area_.scale(s);
   click_area_.move(*(sprite_->position()) * (1.0f - s));
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
bool ButtonForm::clicked(Vector2D position) { return click_area_.point_in_polygon(position); }

void ButtonForm::write(SerializationManager* io)
{
   io->write("BUTTONFORM");
   sprite_->write(io);
   io->write(label_);
   label_sprite_->write(io);
   std::ranges::for_each(click_area_.vertices, [&](const Vector2D& v)
   {
      io->write(v.x);
      io->write(v.y);
   });
}

/********************************************************************************
    GAME OBJECT
********************************************************************************/
GameObject::GameObject(std::string id, bool state)
    : id_{id}, state_{state} {}

void GameObject::write(SerializationManager* io)
{
   io->write(id_);
   io->write(state_);
   form_->write(io);
}
