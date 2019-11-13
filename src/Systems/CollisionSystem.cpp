//
// Created by Cody on 11/6/2019.
//

#include <Engine/GameEngine.hpp>
#include <Components/CollisionComponent.hpp>
#include "CollisionSystem.hpp"

// WIP

void CollisionSystem::update(float ms) {
    for (auto & e1 : *GameEngine::getInstance().getEntityManager()->getEntities()) {
        if (!e1.second->hasComponent<CollisionComponent>())
            continue;
        for (auto & e2 : *GameEngine::getInstance().getEntityManager()->getEntities()) {
            if (!e2.second->hasComponent<CollisionComponent>())
                continue;
            if (e1.first == e2.first)
                continue;

            if (checkCollision(e1.second, e2.second)) {
                auto * e1col = e1.second->getComponent<CollisionComponent>();
                auto * e2col = e2.second->getComponent<CollisionComponent>();
                e1.second->collideWith(e1col->classname, *e2.second);
                e2.second->collideWith(e2col->classname, *e1.second);
            }
        }
    }
}

bool CollisionSystem::checkCollision(std::unique_ptr<ECS::Entity>& e1,std::unique_ptr<ECS::Entity>& e2) {
    auto * e1col = e1->getComponent<CollisionComponent>();
    auto * e2col = e2->getComponent<CollisionComponent>();

    // TODO check for a collision depending on collisionComponent type and other components?
    // Pull bounding box from sprite component?
    return false;
}


