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
    /*auto projectile_it = pickups.begin();
    while(projectile_it != pickups.end()) {
        if ((*projectile_it)->isOffScreen(screen))
        {
            (*projectile_it)->destroy();
            projectile_it = pickups.erase(projectile_it);
            continue;
        } else {
            (*projectile_it)->update(ms);
        }
        ++projectile_it;
    }*/
}

std::vector<Pickup*>* PickupSystem::getPickups() {
	return &pickups;
}

void PickupSystem::pushPickup(Pickup* pk) {
	pickups.emplace_back(pk);
}

void PickupSystem::clear() {
    for (auto* pickup : pickups) {
        pickup->destroy();
    }
}
