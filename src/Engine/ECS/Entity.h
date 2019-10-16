//
// Created by Cody on 10/16/2019.
//

#include <vector>
#include <memory>
#include <bitset>
#include <algorithm>

#ifndef VAPE_ENTITY_H
#define VAPE_ENTITY_H

#include "Component.hpp"

class Component;

constexpr std::size_t maxComponents = 32;

using ComponentId = std::size_t;
using ComponentBitSet = std::bitset<maxComponents>;
using ComponentArray = std::array<Component*, maxComponents>;

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

namespace ECS {
    class Entity {
    private:
        bool active = true;

        std::vector<std::unique_ptr<Component>> components; // Component pointers

        ComponentArray componentArray; // Components indexed by typeId
        ComponentBitSet componentBitSet; // Quick True/False lookup for if has component;

    public:
        void update() {
            for(auto& component : components) component->update();
        };
        void draw() {
            for(auto& component : components) component->draw();
        };

        bool isActive() { return active; };
        void destroy() { active = false; };

        template <typename T> bool hasComponent() const {
            return componentBitSet[getComponentTypeId<T>];
        }

        template <typename T, typename... TArgs> T& addComponent(TArgs&&... mArgs) {
            T* component(new T(std::forward<TArgs>(mArgs)...));
            component->entity = this;
            std::unique_ptr<Component> uPtr{ component };
            components.emplace_back((std::move(uPtr)));

            componentArray[getComponentTypeId<T>()] = component;
            componentBitSet[getComponentTypeId<T>()] = true;

            component->init();
            return *component;
        };

        template<typename T> T& getComponent() const {
            auto ptr(componentArray[getComponentTypeId<T>()]);
            return *static_cast<T*>(ptr);
        }
        // entity.getComponent<PositionCopmonent>().setPosition(...);
    };
}

#endif //VAPE_ENTITY_H
