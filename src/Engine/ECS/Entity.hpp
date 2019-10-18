//
// Created by Cody on 10/16/2019.
//

#include <vector>
#include <memory>
#include <bitset>
#include <algorithm>
#include <common.hpp>
#include <array>

#ifndef VAPE_ENTITY_H
#define VAPE_ENTITY_H

#include "Component.hpp"

namespace ECS {
    constexpr std::size_t maxComponents = 32;

    using ComponentBitSet = std::bitset<maxComponents>;
    using ComponentArray = std::array<Component*, maxComponents>;


    class Entity {
    private:
        bool active = true;

        std::vector<std::unique_ptr<Component>> components; // Component pointers

        ComponentArray componentArray; // Components indexed by typeId
        ComponentBitSet componentBitSet; // Quick True/False lookup for if has component;

    public:
        virtual void update(float ms) {};
        virtual void draw(const mat3& projection) {};

        bool isActive() { return active; };
        virtual void destroy() { active = false; };

        template <typename T> bool hasComponent() const {
            return componentBitSet[getComponentTypeId<T>];
        }

        template <typename T, typename... TArgs> T* addComponent(TArgs&&... mArgs) {
            T* component(new T(std::forward<TArgs>(mArgs)...));
            component->entity = this;
            std::unique_ptr<Component> uPtr{ component };
            components.emplace_back((std::move(uPtr)));

            componentArray[getComponentTypeId<T>()] = component;
            componentBitSet[getComponentTypeId<T>()] = true;

            component->init();

            return component;
        };

        template<typename T> T* getComponent() const {
            auto ptr(componentArray[getComponentTypeId<T>()]);
            return static_cast<T*>(ptr);
        }
        // entity.getComponent<PositionCopmonent>().setPosition(...);
    };
}

#endif //VAPE_ENTITY_H