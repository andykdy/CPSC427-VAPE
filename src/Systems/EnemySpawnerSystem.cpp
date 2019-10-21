//
// Created by Cody on 10/17/2019.
//

#include <Engine/GameEngine.hpp>
#include "EnemySpawnerSystem.hpp"

void EnemySpawnerSystem::update(float ms) {
    time += ms; // TODO game speed
    for (auto & e : *GameEngine::getInstance().getEntityManager()->getEntities()) {

    }
}
