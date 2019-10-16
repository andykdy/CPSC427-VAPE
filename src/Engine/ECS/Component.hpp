//
// Created by Cody on 10/16/2019.
//

#ifndef VAPE_COMPONENT_H
#define VAPE_COMPONENT_H

#include "Entity.h"

namespace ECS {
    class Component {
    public:
        // Owner
        Entity* entity;

        virtual void init() {}
        virtual void update() {}
        virtual void draw() {}

        // Deconstructor
        virtual ~Component() {}
    };
}

#endif //VAPE_COMPONENT_H
