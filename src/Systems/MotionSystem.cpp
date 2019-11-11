//
// Created by Cody on 10/20/2019.
//

#include <Engine/GameEngine.hpp>
#include <iostream>
#include <algorithm>
#include <Components/MotionComponent.hpp>
#include <Components/BoundaryComponent.hpp>
#include "MotionSystem.hpp"
#include "Engine/ECS/ECS.hpp"

void MotionSystem::update(float ms) {
    for (auto & e : *GameEngine::getInstance().getEntityManager()->getEntities()) {
        if (e.second->hasComponent<MotionComponent>()) {
            auto* motion = e.second->getComponent<MotionComponent>();

            accelerate(motion);

            // TODO maybe add some way to avoid being affected by slowdown, or a separate component for vamp mode slowdown
            vec2 step = {motion->velocity.x * (ms / 1000), motion->velocity.y * (ms / 1000)};

            // Handle Boundaries
            if (e.second->hasComponent<BoundaryComponent>()){
                auto* boundary = e.second->getComponent<BoundaryComponent>();
                motion->position.x = std::min(std::max(motion->position.x + step.x, boundary->minX), boundary->maxX);
                motion->position.y = std::min(std::max(motion->position.y + step.y, boundary->minY), boundary->maxY);
            } else {
                motion->position.x = motion->position.x + step.x;
                motion->position.y = motion->position.y + step.y;
            }

            // Decay velocity
            if (motion->friction != 0) {
                if (motion->velocity.x > 0.f)
                    motion->velocity.x -= motion->friction * motion->velocity.x;
                else if (motion->velocity.x < 0.f)
                    motion->velocity.x += -motion->friction * motion->velocity.x;

                if (motion->velocity.y > 0.f)
                    motion->velocity.y -= motion->friction * motion->velocity.y;
                else if (motion->velocity.y < 0.f)
                    motion->velocity.y += -motion->friction * motion->velocity.y;
            }
        }
    }
}

void MotionSystem::accelerate(MotionComponent* motion) {
	float max = motion->maxVelocity;
	if (max >= 0) {
		float newX = motion->velocity.x + motion->acceleration.x;
		if (newX > max) newX = max;
		if (newX < -max) newX = -max;

		float newY = motion->velocity.y + motion->acceleration.y;
		if (newY > max) newY = max;
		if (newY < -max) newY = -max;

		motion->velocity.x = newX;
		motion->velocity.y = newY;
	}
}