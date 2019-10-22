//
// Created by Cody on 10/16/2019.
//

#include <common.hpp>
#include <Engine/ECS/ECS.hpp>

#ifndef VAPE_MOTIONCOMPONENT_HPP
#define VAPE_MOTIONCOMPONENT_HPP

class MotionComponent : public ECS::Component {
public:
    vec2 position = {0,0};
    float radians = 0.f;

    vec2 velocity = {0,0};
    vec2 acceleration = {0,0};
    float maxVelocity = -1; // Negative = not checked
    float friction = 0.f;
};

#endif //VAPE_MOTIONCOMPONENT_HPP