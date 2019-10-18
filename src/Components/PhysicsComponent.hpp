//
// Created by Cody on 10/16/2019.
//

#include <common.hpp>
#include <Engine/ECS/ECS.hpp>

#ifndef VAPE_PHYSICSCOMPONENT_HPP
#define VAPE_PHYSICSCOMPONENT_HPP

class PhysicsComponent : public ECS::Component {
public:
    vec2 scale = { 0, 0 };
};

#endif //VAPE_PHYSICSCOMPONENT_HPP