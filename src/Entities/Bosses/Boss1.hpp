//
// Created by cody on 9/26/19.
//

#ifndef VAPE_BOSS1_HPP
#define VAPE_BOSS1_HPP

#include <Engine/GameEngine.hpp>
#include <Entities/Projectiles and Damaging/bullet.hpp>
#include <vector>
#include "common.hpp"
#include "Boss.hpp"

enum class Direction {left, right};

class Boss1 : public Boss {
    // Shared between all turtles, no need to load one for each instance
    static Texture boss1_texture;

public:
    // Creates all the associated render resources and default transform
    bool init(vec2 screen) override;

    // Releases all the associated resources
    void destroy() override;

    // Update Boss due to current
    // ms represents the number of milliseconds elapsed from the previous update() call
    void update(float ms) override;

    // Renders the Boss
    // projection is the 2D orthographic projection matrix
    void draw(const mat3& projection) override;

    // Returns the current turtle position
    vec2 get_position()const override;

    // Sets the new Boss position
    void set_position(vec2 position) override;

    // Returns the Boss' bounding box for collision detection, called by collides_with()
    vec2 get_bounding_box() const override;

    void addDamage(int damage) override;

    bool collidesWith(Vamp vamp) override;

    bool checkCollision(vec2 pos, vec2 box) const override;

private:
    float m_speed;
    Direction dir; // Direction right or left
    unsigned int m_burst_count;
    float m_burst_cooldown;
    float m_bullet_cooldown;
    float m_damage_effect_cooldown;

    // Alternate update functions to run depending on state
    void state1Update(float ms);
    void state2Update(float ms);

    void spawnBullet();
};


#endif //VAPE_BOSS1_HPP
