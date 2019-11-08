//
// Created by Cody on 11/1/2019.
//


#include "EntityManager.hpp"

namespace ECS {
    void EntityManager::update(float ms) {
        {
            auto it = entities.begin();
            while (it != entities.end()) {
                // TODO it->second->update(ms);
                if (!it->second->isActive())
                    it = entities.erase(it);
                else
                    it++;
            }
        }
    }

    void EntityManager::draw(const mat3 &projection) {
        for (auto & e : entities) e.second->draw(projection);
    }
}