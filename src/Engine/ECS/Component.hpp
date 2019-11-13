//
// Created by Cody on 10/16/2019.
//

#ifndef VAPE_COMPONENT_H
#define VAPE_COMPONENT_H

#include "Entity.hpp"

namespace ECS {
    class Entity;

    class Component {
    public:
        // Owner
        Entity* entity;

        virtual void init() {};

        // Deconstructor
        virtual ~Component() = default;
    };
}

#endif //VAPE_COMPONENT_H
