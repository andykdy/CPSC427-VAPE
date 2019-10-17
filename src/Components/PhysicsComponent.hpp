//
// Created by Cody on 10/16/2019.
//

#include <common.hpp>
#include <Engine/ECS/ECS.hpp>

#ifndef VAPE_PHYSICSCOMPONENT_HPP
#define VAPE_PHYSICSCOMPONENT_HPP

class PhysicsComponent : public ECS::Component {
private:
    vec2 scale;
public:
    void init() override { scale.x = 0;scale.y = 0; };

    const vec2 &getScale() const { return scale; };
    const float getScaleX() const { return scale.x; };
    const float getScaleY() const { return scale.y; };

    void setScale(float x, float y) { scale.x = x; scale.y = y; }
};

#endif //VAPE_PHYSICSCOMPONENT_HPP