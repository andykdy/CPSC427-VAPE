//
// Created by Cody on 11/20/2019.
//

#ifndef VAPE_WEAPON_HPP
#define VAPE_WEAPON_HPP

#include <Entities/Projectiles and Damaging/Projectile.hpp>

class Projectile;

class Weapon {
protected:
    std::vector<Projectile*> m_projectiles;
public:
    virtual void init() {};

    virtual void fire(const vec2& origin_position, float origin_rotation) = 0;

    virtual void update(float ms) = 0;

    virtual void draw(const mat3& projection) = 0;

    inline std::vector<Projectile*>* getProjectiles() { return &m_projectiles;};

    virtual void destroy();
};

#endif //VAPE_WEAPON_HPP
