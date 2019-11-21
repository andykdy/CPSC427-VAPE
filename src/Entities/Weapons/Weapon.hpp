//
// Created by Cody on 11/20/2019.
//

#ifndef VAPE_WEAPON_HPP
#define VAPE_WEAPON_HPP

#include <Entities/Projectiles and Damaging/Projectile.hpp>

class Projectile;

class Weapon {
public:
    virtual void init() {};

    virtual Projectile* fire(const vec2& origin_position, float origin_rotation) = 0;

    virtual void update(float ms) = 0;

    virtual void destroy() = 0;
};

#endif //VAPE_WEAPON_HPP