//
// Created by Cody on 11/23/2019.
//

#ifndef VAPE_PROJECTILESYSTEM_HPP
#define VAPE_PROJECTILESYSTEM_HPP


#include <Engine/ECS/System.hpp>

class ProjectileSystem : public ECS::System {
public:
    std::vector<Projectile*> friendly_projectiles;
    std::vector<Projectile*> hostile_projectiles;
    void update(float ms) override;
    void clear();
};


#endif //VAPE_PROJECTILESYSTEM_HPP
