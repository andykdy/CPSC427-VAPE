//
// Created by Cody on 10/16/2019.
// Based on:
// https://www.youtube.com/watch?v=NTWSeQtHZ9M
// https://www.youtube.com/watch?v=XsvI8Sng6dk
//

#ifndef VAPE_ECS_HPP
#define VAPE_ECS_HPP

#include <vector>
#include <memory>
#include <bitset>
#include <algorithm>


class Component;
class Entity;

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

class Manager {
private:
    std::vector<std::unique_ptr<Entity>> entities;

public:
    void update() {
        for (auto & e : entities) e->update();
    }

    void draw() {
        for (auto & e : entities) e->draw();
    }

    // https://en.cppreference.com/w/cpp/algorithm/remove
    void refresh() {
        // This erases from the entities vector based on which entities are not active
        // see remove_if docs for more details, but it basically just shifts all
        // inactive elements end of the vector, and returns an iterator which erase uses.
        entities.erase(std::remove_if(entities.begin(), entities.end(), [](const std::unique_ptr<Entity> &entity) {
            return !entity->isActive();
        }), entities.end());
    }

    Entity& addEntity() {
        Entity* e = new Entity();
        std::unique_ptr<Entity> uPtr{ e };
        entities.emplace_back(std::move(uPtr));
    };
};


#endif //VAPE_ECS_HPP