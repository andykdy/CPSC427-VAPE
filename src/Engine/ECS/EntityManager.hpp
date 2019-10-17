//
// Created by Cody on 10/16/2019.
//

#ifndef VAPE_ENTITYMANAGER_HPP
#define VAPE_ENTITYMANAGER_HPP

#include "Entity.hpp"

namespace ECS {
    class EntityManager {
    private:
        std::vector<std::unique_ptr<Entity>> entities;

    public:
        void update(float ms) {
            for (auto & e : entities) {
                e->update(ms);
            }
            refresh();
        }

        void draw(const mat3& projection) {
            for (auto & e : entities) e->draw(projection);
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

        template <typename T, typename... TArgs> T& addEntity(TArgs&&... mArgs) {
            T* e(new T(std::forward<TArgs>(mArgs)...));
            std::unique_ptr<Entity> uPtr{ e };
            entities.emplace_back(std::move(uPtr));
            return *e;
        }
    };
}

#endif //VAPE_ENTITYMANAGER_HPP
