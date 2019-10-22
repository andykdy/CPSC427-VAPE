//
// Created by cody on 9/26/19.
//

#ifndef VAPE_BOSS1_HPP
#define VAPE_BOSS1_HPP

#include <Engine/GameEngine.hpp>
#include <Entities/bullet.hpp>
#include <vector>
#include "common.hpp"

enum class Direction {left, right};

class Boss1 : public EntityOld {
    // Shared between all turtles, no need to load one for each instance
    static Texture boss1_texture;

public:
    // Creates all the associated render resources and default transform
    bool init();

    // Releases all the associated resources
    void destroy();

    // Update Boss due to current
    // ms represents the number of milliseconds elapsed from the previous update() call
    void update(float ms);

    // Renders the Boss
    // projection is the 2D orthographic projection matrix
    void draw(const mat3& projection) override;

    // Returns the current turtle position
    vec2 get_position()const;

    // Sets the new Boss position
    void set_position(vec2 position);

    // Returns the Boss' bounding box for collision detection, called by collides_with()
    vec2 get_bounding_box() const;

    std::vector<Bullet> bullets;

    int getHealth() const;

    void addDamage(int damage);

    bool is_alive();
    void kill();

private:
    int health;
    Direction dir; // Direction right or left
    bool m_is_alive;
    float m_bullet_cooldown;

    // Alternate update functions to run depending on state
    void state1Update(float ms);
    void state2Update(float ms);

    void spawnBullet();
};


#endif //VAPE_BOSS1_HPP
