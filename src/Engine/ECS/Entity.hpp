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
    class Component;
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

    using EntityId = std::size_t;

    constexpr std::size_t maxComponents = 32;

    using ComponentBitSet = std::bitset<maxComponents>;
    using ComponentArray = std::array<Component*, maxComponents>;
    using ComponentPtrArray = std::array<std::unique_ptr<Component>, maxComponents>;


    class Entity {
        friend class EntityManager;
    private:
        EntityId id;
        bool active = true;

        std::vector<std::unique_ptr<Component>> components; // Component pointers

        ComponentArray componentArray{}; // Components indexed by typeId
        ComponentBitSet componentBitSet; // Quick True/False lookup for if has component;
    public:
        EntityId getId() const { return id; }

        virtual void update(float ms) {};
        virtual void draw(const mat3& projection) {};
        virtual void destroy() { active = false; };
        virtual void collideWith(const char* typeName, const Entity& other) {};
		virtual vec2 get_position()const { return  { 0.f,0.f }; };
		virtual vec2 get_bounding_box()const { return  { 0.f,0.f }; };

        bool isActive() const { return active; };

        template <typename T> bool hasComponent() const {
            return componentBitSet[getComponentTypeId<T>()];
        };

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
        };
    };
}

#endif //VAPE_ENTITY_H
