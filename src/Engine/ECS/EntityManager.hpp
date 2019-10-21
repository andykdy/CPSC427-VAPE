//
// Created by Cody on 10/16/2019.
//

#ifndef VAPE_ENTITYMANAGER_HPP
#define VAPE_ENTITYMANAGER_HPP

#include <unordered_map>
#include "Entity.hpp"

namespace ECS {
    template<typename T, typename... Args>
    std::unique_ptr<T> make_unique(Args&&... args) {
        return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
    }

    class EntityManager {
    private:
        EntityId idCounter;
        std::unordered_map<EntityId, std::unique_ptr<Entity>> entities;

    public:
        void update(float ms) {
            auto it = entities.begin();
            while (it != entities.end()) {
                if (!it->second->isActive())
                    it = entities.erase(it);
                else
                    it++;
            }
        }

        void draw(const mat3& projection) {
            for (auto & e : entities) e.second->draw(projection);
        }

        template <typename T, typename... TArgs> T& addEntity() {
            auto id = idCounter;
            ++idCounter;

            T* e(new T(id));
            std::unique_ptr<Entity> uPtr{ e };
            entities.emplace(id, std::move(uPtr));
            return *e;
        }

        template <typename T> T& getEntity(EntityId id) {
            return entities[id];
        }

        /*
        void removeEntity(EntityId id) {
            entities.erase(id);
        }
         */

        std::unordered_map<EntityId, std::unique_ptr<Entity>> * getEntities() {
            return &entities;
        }

        void reset() {
            entities.clear();
        }
    };
}

#endif //VAPE_ENTITYMANAGER_HPP
