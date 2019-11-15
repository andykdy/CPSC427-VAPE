//
// Created by Cody on 11/14/2019.
//

#ifndef VAPE_BUTTON_HPP
#define VAPE_BUTTON_HPP

#include <Engine/ECS/Entity.hpp>

class Button : public ECS::Entity {
private:
    bool active = true;
    bool selected = false;
public:
    virtual bool init(const vec2& position, const vec2&scale, float rotation) = 0;
    virtual void destroy() override = 0;
    virtual void update(float ms, const vec2& mouse_position) = 0;
    virtual void draw(const mat3 &projection) override = 0;
    virtual bool isWithin(const vec2& mouse_position) = 0;
    virtual void doAction() {};
    virtual void disable() { active = false; };
    virtual void enable() {active = true; };
    virtual bool isActive() { return active; };
    virtual void select() { selected = true; };
    virtual void delesect() { selected = false; };
    virtual bool isSelected() { return selected; };
    virtual vec2 getPosition() = 0;
};

#endif //VAPE_BUTTON_HPP
