//
// Created by Cody on 11/23/2019.
//

#include <Engine/GameEngine.hpp>
#include <Components/PlayerComponent.hpp>
#include <Components/HealthComponent.hpp>
#include <Components/EnemyComponent.hpp>
#include <Entities/Projectiles and Damaging/Projectile.hpp>
#include "ProjectileSystem.hpp"

void ProjectileSystem::update(float ms) {

    // Get screen size
    int w, h;
    glfwGetFramebufferSize(GameEngine::getInstance().getM_window(), &w, &h);
    vec2 screen = { (float)w / GameEngine::getInstance().getM_screen_scale(), (float)h / GameEngine::getInstance().getM_screen_scale() };

    // Update bullets, removing out of screen bullets
    auto projectile_it = friendly_projectiles.begin();
    while(projectile_it != friendly_projectiles.end()) {
        if ((*projectile_it)->isOffScreen(screen))
        {
            (*projectile_it)->destroy();
            projectile_it = friendly_projectiles.erase(projectile_it);
            continue;
        } else {
            (*projectile_it)->update(ms);
        }

        ++projectile_it;
    }

    // Update bullets, removing out of screen bullets
    projectile_it = hostile_projectiles.begin();
    while(projectile_it != hostile_projectiles.end()) {
        if ((*projectile_it)->isOffScreen(screen))
        {
            (*projectile_it)->destroy();
            projectile_it = hostile_projectiles.erase(projectile_it);
            continue;
        } else {
            (*projectile_it)->update(ms);
        }

        ++projectile_it;
    }


    /* This was me playing around with an actual full on projectile ECS system, but it would require a lot of refactoring to work... so nevermind
    std::vector<Projectile*> hostile_projectiles;
    std::vector<Projectile*> friendly_projectiles;
    std::vector<ECS::Entity*> players;
    std::vector<ECS::Entity*> enemies;

    for (auto & e : *GameEngine::getInstance().getEntityManager()->getEntities()) {
        if (e.second->hasComponent<ProjectileComponent>()) {
            if (e.second->getComponent<ProjectileComponent().m_hostile)
                hostile_projectiles.push_back(e.second);
            else
                friendly_projectiles.push_back(e.second);
        } else if (e.second->hasComponent<PlayerComponent>() && e.second->hasComponent<HealthComponent>()) {
            players.push_back(e.second);
        } else if (e.second->hasComponent<EnemyComponent>()) { // TODO use HealthComponent on enemies

        }
    }

    // Check hostile - player collisions
    auto hostile_bullet_it = hostile_projectiles.begin();
    while (hostile_bullet_it != hostile_projectiles.end()) {
        for (auto player : players) {
            if ((*hostile_bullet_it)->collides_with(*player))
            {
                (*hostile_bullet_it)->destroy();
                hostile_bullet_it = hostile_projectiles.erase(hostile_bullet_it);
                if (m_player->is_alive() && m_player->get_iframes() <= 0.f) {
                    m_player->set_iframes(500.f);
                    lose_health(DAMAGE_BOSS);
                }
                break;
            } else {
                ++bullet_it;
            }
        }
    }
     */
}

void ProjectileSystem::clear() {
    for (auto* projectile : friendly_projectiles) {
        projectile->destroy();
    }
    for (auto* projectile : hostile_projectiles) {
        projectile->destroy();
    }
}
