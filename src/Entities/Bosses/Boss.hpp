//
// Created by Cody on 11/5/2019.
//

#ifndef VAPE_BOSS_HPP
#define VAPE_BOSS_HPP

#include <common.hpp>
#include <Engine/ECS/Entity.hpp>
#include <Entities/bullet.hpp>

class Boss : public ECS::Entity { // TODO refactor to new entity
    // Shared between all turtles, no need to load one for each instance
    static Texture boss_texture;
protected:
    int health = 1;
    bool m_is_alive = true;
public:
    // Creates all the associated render resources and default transform
    virtual bool init() = 0;

    // Releases all the associated resources
    void destroy() override = 0;

    // Update Boss due to current
    // ms represents the number of milliseconds elapsed from the previous update() call
    void update(float ms) override = 0;

    // Renders the Boss
    // projection is the 2D orthographic projection matrix
    void draw(const mat3& projection) override = 0;

    // Returns the current position
    virtual vec2 get_position()const = 0;

    // Sets the new position
    virtual void set_position(vec2 position) = 0;

    // Returns the Boss' bounding box for collision detection, called by collides_with()
    virtual vec2 get_bounding_box() const = 0;

    // TODO bullets / damaging objects superclass so that level can loop through them and check for collisions with player? or make new component/system

    inline int getHealth() const { return health; };

    inline void addDamage(int damage) { health -= damage; };

    inline bool is_alive() {return m_is_alive;};
    inline void kill() { m_is_alive = false; };
};

#endif //VAPE_BOSS_HPP
