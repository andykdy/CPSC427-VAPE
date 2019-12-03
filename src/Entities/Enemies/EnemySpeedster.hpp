//
// Created by Tanha Kabir on 2019-12-02.
//

#ifndef VAPE_ENEMYSPEEDSTER_HPP
#define VAPE_ENEMYSPEEDSTER_HPP

#include <Entities/Enemies/Enemy.hpp>
#include "common.hpp"

class EnemySpeedster: public Enemy {
    // Shared between all turtles, no need to load one for each instance
    static Texture texture;
public:
// Creates all the associated render resources and default transform
    bool init() override;

    // Releases all the associated resources
    void destroy() override;

    // Update turtle due to current
    // ms represents the number of milliseconds elapsed from the previous update() call
    void update(float ms) override;

    // Renders the salmon
    // projection is the 2D orthographic projection matrix
    void draw(const mat3& projection) override;

    // Returns the current turtle position
    vec2 get_position()const override;

    // Sets the new turtle position
    void set_position(vec2 position) override;

    // Returns the turtle' bounding box for collision detection, called by collides_with()
    vec2 get_bounding_box() const override;

    void set_velocity(vec2 velocity) override;

private:
    float m_stationary_cooldown_ms;
    bool m_was_stationed;

    float m_rotate_cooldown_ms;
    float m_rotation_direction;
};

#endif //VAPE_ENEMYSPEEDSTER_HPP
