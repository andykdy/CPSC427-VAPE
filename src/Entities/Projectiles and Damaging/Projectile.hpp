//
// Created by test on 2019-11-07.
//

#ifndef VAPE_PROJECTILE_HPP
#define VAPE_PROJECTILE_HPP

#include <Engine/ECS/Entity.hpp>

class Projectile : public ECS::Entity {
protected:
    int m_damage = 0;
    bool m_hostile = false;
public:
    virtual bool init(vec2 position, float rotation) = 0; // TODO, probably uneccessary
    virtual vec2 get_position() const = 0; // TODO probably unneccessary, just get from components
    inline int getDamage() const { return m_damage; };
    inline bool isHostile() const { return m_hostile; };
};

#endif //VAPE_PROJECTILE_HPP
