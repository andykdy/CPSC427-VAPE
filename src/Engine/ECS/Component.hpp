//
// Created by Cody on 10/16/2019.
//

#ifndef VAPE_COMPONENT_H
#define VAPE_COMPONENT_H

#include "Entity.hpp"

class Entity;

namespace ECS {
    using ComponentId = std::size_t;

    // Returns a unique, incremeneted Id
    inline ComponentId getComponentTypeId() {
        static ComponentId lastId = 0;
        return lastId++;
    };

    // Returns a unique id for each type T of component;
    template <typename T> inline ComponentId getComponentTypeId() noexcept {
        static ComponentId typeId = getComponentTypeId();
        return typeId;
    }

    class Component {
    public:
        // Owner
        Entity* entity;

        // Deconstructor
        virtual ~Component() = default;
    };
}

#endif //VAPE_COMPONENT_H
