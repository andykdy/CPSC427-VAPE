//
// Created by Cody on 10/16/2019.
//

#include <common.hpp>
#include <Engine/ECS/ECS.hpp>

#ifndef VAPE_MOTIONCOMPONENT_HPP
#define VAPE_MOTIONCOMPONENT_HPP

class MotionComponent : public Component {
private:
    vec2 position;
    float radians;
    float speed;
public:
    void init() override {
        position.x = 0;
        position.y = 0;
        radians = 0;
        speed = 0;
    }

    void update() override {

    }

    const vec2 &getPosition() const { return position; };
    const float x() const { return position.x; };
    const float y() const { return position.y; };

    void setPosition(float x, float y) { position.x = x; position.y = y; };

    float getRadians() const { return radians; };

    void setRadians(float radians) { MotionComponent::radians = radians; };

    float getSpeed() const { return speed; }

    void setSpeed(float speed) { MotionComponent::speed = speed; };
};

#endif //VAPE_MOTIONCOMPONENT_HPP