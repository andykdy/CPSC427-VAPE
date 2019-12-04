//
// Created by Andy on 12/03/2019.
//

#include <Engine/GameEngine.hpp>
#include <Components/PlayerComponent.hpp>
#include <Components/HealthComponent.hpp>
#include <Components/EnemyComponent.hpp>
#include <Entities/Pickups/Pickup.hpp>
#include "PickupSystem.hpp"

void PickupSystem::update(float ms) {

    // Get screen size
    int w, h;
    glfwGetFramebufferSize(GameEngine::getInstance().getM_window(), &w, &h);
    vec2 screen = { (float)w / GameEngine::getInstance().getM_screen_scale(), (float)h / GameEngine::getInstance().getM_screen_scale() };

    // Update bullets, removing out of screen bullets
    auto pickup_it = pickups.begin();
    while(pickup_it != pickups.end()) {
        if ((*pickup_it)->isOffScreen(screen))
        {
            (*pickup_it)->destroy();
			pickup_it = pickups.erase(pickup_it);
            continue;
        } else {
            (*pickup_it)->update(ms);
        }
        ++pickup_it;
    }
}

std::vector<Pickup*>* PickupSystem::getPickups() {
	return &pickups;
}


void PickupSystem::clear() {
    for (auto* pickup : pickups) {
        pickup->destroy();
    }
}
