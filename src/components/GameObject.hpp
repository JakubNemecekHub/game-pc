#pragma once

#include <string>
#include <vector>
#include <memory>

#include <SDL2/SDL.h>

#include "../utils/Mouse.hpp"
#include "../math/Polygon.hpp"

class RenderManager;
class TextManager;
class State;
class Sprite;


class Form
{
public:

    bool debug { false };

    virtual  void x(float x) = 0;
    virtual  void y(float y) = 0;
    virtual  void position(float x, float y) = 0;
    virtual float x() = 0;
    virtual float y() = 0;
    virtual  void move(float dx, float dy) = 0;
    virtual  void set_scale(float s) = 0;
    virtual  void scale(float s) = 0;
    virtual float scale() = 0;
    virtual  void z_index(int z) = 0;
    virtual   int z_index() = 0;
    virtual void depiction(std::string id) = 0;

    virtual  void update(RenderManager* renderer, int dt) = 0;
    virtual  bool clicked(float x, float y) = 0;

};

class Body : public Form
{
private:

    Sprite* sprite_;

public:

    Body(Sprite* sprite) : sprite_{sprite} {}

     void x(float x) override;
     void y(float y) override;
     void position(float x, float y) override;
    float x() override;
    float y() override;
     void move(float dx, float dy) override;
     void set_scale(float s) override;
     void scale(float s) override;
    float scale() override;
     void z_index(int z) override;
      int z_index() override;
    void depiction(std::string id) override;

    void update(RenderManager* renderer, int dt) override;
    bool clicked(float x, float y) override;

};

class RigidBody : public Form
{
private:

    Sprite* sprite_;
    Polygon click_area_;

public:

    RigidBody(Sprite* sprite, std::vector<std::vector<float>> vertices);

     void x(float x) override;
     void y(float y) override;
     void position(float x, float y) override;
    float x() override;
    float y() override;
     void move(float dx, float dy) override;
     void set_scale(float s) override;
     void scale(float s) override;
    float scale() override;
     void z_index(int z) override;
      int z_index() override;
    void depiction(std::string id) override;

    void update(RenderManager* renderer, int dt) override;
    bool clicked(float x, float y) override;

};

class Trigger : public Form
{
private:

    Polygon click_area_;
    float scale_ { 1.0f };

public:

    Trigger(std::vector<std::vector<float>> vertices);

     void x(float x) override;
     void y(float y) override;
     void position(float x, float y) override;
    float x() override;
    float y() override;
     void move(float dx, float dy) override;
     void set_scale(float s) override;
     void scale(float s) override;
    float scale() override;
     void z_index(int z) override;
      int z_index() override;
    void depiction(std::string id) override;

    void update(RenderManager* renderer, int dt) override;
    bool clicked(float x, float y) override;

};

class ButtonForm : public Form
{
private:

    Sprite* sprite_;
    std::string label_;
    std::unique_ptr<Sprite> label_sprite_;
    Polygon click_area_;

public:

    ButtonForm(Sprite* sprite, std::vector<std::vector<float>> vertices,
               TextManager* text, std::string label);

     void x(float x) override;
     void y(float y) override;
     void position(float x, float y) override;
    float x() override;
    float y() override;
     void move(float dx, float dy) override;
     void set_scale(float s) override;
     void scale(float s) override;
    float scale() override;
     void z_index(int z) override;
      int z_index() override;
    void depiction(std::string id) override;

    void update(RenderManager* renderer, int dt) override;
    bool clicked(float x, float y) override;

};

class GameObject
{
protected:

    std::string id_;
    std::unique_ptr<Form> form_;
    bool state_;

public:

    GameObject() {}
    GameObject(std::string id, bool state = false);
    ~GameObject() {}

    inline std::string id()           { return id_;         }
    inline bool state()               { return state_;      }
    inline void state(bool new_state) { state_ = new_state; }

    inline  void x(float x)                 { form_->x(x);             }
    inline  void y(float y)                 { form_->y(y);             }
    inline  void position(float x, float y) { form_->position(x, y);   }
    inline float x()                        { return form_->x();       }
    inline float y()                        { return form_->y();       }
    inline  void move(float dx, float dy)   { form_->move(dx, dy);     }
    inline  void set_scale(float s)         { form_->set_scale(s);     }
    inline  void scale(float s)             { form_->scale(s);         }
    inline float scale()                    {
        return form_->scale();   }
    inline  void z_index(int z)             { form_->z_index(z);       }
    inline   int z_index()                  { return form_->z_index(); }
    inline  void depiction(std::string id)  { form_->depiction(id);   }
    
    virtual void update(RenderManager* renderer, int dt) = 0;
    inline bool clicked(float x, float y) { return form_->clicked(x, y) && state_; }

    // Game Logic

    virtual void accept_click(State* handler, Mouse::Status mouse) = 0;
    virtual void accept_over(State* handler, Mouse::Status mouse) = 0;
    virtual void accept_drag(State* handler, Mouse::Status mouse) = 0;

    // DEBUG

    inline void show_attributes() { form_->debug = true; }

};
 