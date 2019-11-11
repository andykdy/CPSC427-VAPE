//
// Created by test on 2019-11-07.
//

#ifndef VAPE_PROJECTILE_HPP
#define VAPE_PROJECTILE_HPP

#include <Engine/ECS/Entity.hpp>
#include <Entities/Player.hpp>
#include <Entities/Bosses/Boss.hpp>


// Forward declarations
class Player;
class Boss1;
class Enemy;

class Projectile : public ECS::Entity {
protected:
    int m_damage = 0;
    bool m_hostile = false;
public:
    virtual bool init(vec2 position, float rotation) = 0; // TODO, probably uneccessary
    virtual vec2 get_position() const = 0; // TODO probably unneccessary, just get from components
    inline int getDamage() const { return m_damage; };
    inline bool isHostile() const { return m_hostile; };
    virtual bool collides_with(const Player &player) = 0; // TODO temp?
    virtual bool collides_with(const Enemy &turtle) = 0; // TODO temp?
    virtual bool collides_with(const Boss &boss) = 0; // TODO temp?

    virtual bool isOffScreen(const vec2& screen) = 0;
};

#endif //VAPE_PROJECTILE_HPP
