//
// Created by Cody on 11/1/2019.
//

#include "SystemManager.hpp"

namespace ECS {

    void SystemManager::update(float ms) {
        for (auto & s : systems) s->update(ms);
        refresh();
    }

    // https://en.cppreference.com/w/cpp/algorithm/remove
    void SystemManager::refresh() {
        // This erases from the entities vector based on which entities are not active
        // see remove_if docs for more details, but it basically just shifts all
        // inactive elements end of the vector, and returns an iterator which erase uses.
        systems.erase(std::remove_if(systems.begin(), systems.end(), [this](const std::unique_ptr<System> &system) {
            if (!system->isActive()){
                systemBitSet[system->id] = false;
            }
            return !system->isActive();
        }), systems.end());
    }

    void SystemManager::clear() {
        systems.clear();
        systemBitSet.reset();
    }
}
