//
// Created by Cody on 10/16/2019.
//

#include <common.hpp>
#include <Engine/ECS/ECS.hpp>

#ifndef VAPE_PHYSICSCOMPONENT_HPP
#define VAPE_PHYSICSCOMPONENT_HPP

class PhysicsComponent : public Component {
private:
    vec2 scale;
public:
    void init() override {
        scale.x = 0;
        scale.y = 0;
    }

    void update() override {

    }

    const vec2 &getScale() const {
        return scale;
    }

    void setScale(const vec2 &scale) {
        PhysicsComponent::scale = scale;
    }
};

#endif //VAPE_PHYSICSCOMPONENT_HPP