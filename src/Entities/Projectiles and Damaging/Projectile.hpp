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
class Boss;
class Enemy;

class Projectile : public ECS::Entity {
protected:
    int m_damage = 0;
    bool m_hostile = false;
    bool m_erase_on_collide = true; // Bullets will want to be erased, but things like lasers do not
public:
    virtual bool init(vec2 position, float rotation, bool hostile, int damage) = 0;
    virtual vec2 get_position() const = 0;
    inline int getDamage() const { return m_damage; };
    inline bool isHostile() const { return m_hostile; };
    inline void setHostile(bool hostile) { m_hostile = hostile; };
    inline bool shouldErase() { return m_erase_on_collide; };
    virtual bool collides_with(const Player &player) = 0;
    virtual bool collides_with(const Enemy &turtle) = 0;
    virtual bool collides_with(const Boss &boss) = 0;

    virtual bool isOffScreen(const vec2& screen) = 0;

    virtual vec2 get_bounding_box() const { return {-1,-1}; };
};

#endif //VAPE_PROJECTILE_HPP
