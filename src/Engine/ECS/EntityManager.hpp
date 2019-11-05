//
// Created by Cody on 10/16/2019.
//

#ifndef VAPE_ENTITYMANAGER_HPP
#define VAPE_ENTITYMANAGER_HPP

#include <unordered_map>
#include "Entity.hpp"

namespace ECS {
    class EntityManager {
    private:
        EntityId idCounter;
        std::unordered_map<EntityId, std::unique_ptr<Entity>> entities;

    public:
        void update(float ms);
        void draw(const mat3& projection);

        template <typename T, typename... TArgs> T& addEntity(TArgs&&... mArgs) {
            T* e(new T(std::forward<TArgs>(mArgs)...));
            auto id = idCounter++;
            e->id = id;
            std::unique_ptr<Entity> uPtr{ e };
            entities.emplace(id, std::move(uPtr));
            return *e;
        };

        template <typename T> T& getEntity(EntityId id) { return *(entities[id]); }

        void removeEntity(EntityId id) { entities.erase(id); }

        // TODO getter to just get ones with specific components?
        std::unordered_map<EntityId, std::unique_ptr<Entity>> * getEntities() { return &entities; }

        void clear() { entities.clear(); }
    };
}

#endif //VAPE_ENTITYMANAGER_HPP
