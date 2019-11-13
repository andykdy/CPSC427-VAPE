//
// Created by Cody on 11/6/2019.
//

#ifndef VAPE_COLLISIONCOMPONENT_HPP
#define VAPE_COLLISIONCOMPONENT_HPP

#include <Engine/ECS/Component.hpp>

enum collidableType {
    none,
    radius,
    aabb,
    complex,
}; // I guess just pull the relevant data required from other components, mainly SpriteComponent?

class CollisionComponent : public ECS::Component {
public:
    collidableType type = collidableType::none;
    const char* classname;

    template <typename T> void setClass() { // Uhh I really don't know if this will work as intended..
        classname = typeid(T).name();
    }
};


#endif //VAPE_COLLISIONCOMPONENT_HPP
