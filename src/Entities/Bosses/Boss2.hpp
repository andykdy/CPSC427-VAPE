//
// Created by Cody on 11/7/2019.
//

#ifndef VAPE_BOSS2_HPP
#define VAPE_BOSS2_HPP


#include <Entities/Debugging/DebugDot.hpp>
#include <Levels/Level.hpp>
#include "Boss.hpp"

struct AttackPattern {
    bool lasers[6];
    float startRotations[6];
    float targetRotations[6]; // TODO: vectors of targets for more complexity?
    float rotationSpeeds[6];
    float chargeTime[6];
    float fireTime[6];
    float nextPatternDelay;

    std::vector<Levels::Wave> waves;

    bool operator==(const AttackPattern &rhs) const;

    bool operator!=(const AttackPattern &rhs) const;
};

class Laser;

class Boss2 : public Boss {
    // Shared between all turtles, no need to load one for each instance
    static Texture boss2_texture;

public:
    // Creates all the associated render resources and default transform
    bool init(vec2 screen) override;

    // Releases all the associated resources
    void destroy() override;

    // Update Boss due to current
    // ms represents the number of milliseconds elapsed from the previous update() call
    void update(float ms) override;

    void choosePattern(const std::vector<AttackPattern>& patterns);

    // Renders the Boss
    // projection is the 2D orthographic projection matrix
    void draw(const mat3 &projection) override;

    // Returns the current turtle position
    vec2 get_position() const override;

    // Sets the new Boss position
    void set_position(vec2 position) override;

    // Returns the Boss' bounding box for collision detection, called by collides_with()
    vec2 get_bounding_box() const override;

    void addDamage(int damage) override;

    bool collidesWith(const Vamp& vamp) override;

    bool collidesWith(const Player &player) override;

    bool checkCollision(vec2 pos, vec2 box) const override;

private:
    DebugDot m_dot;
    std::vector<Vertex> m_vertices;
    float m_damage_effect_cooldown;

    std::vector<Laser*> m_lasers;

    float m_pattern_timer;
    AttackPattern m_pattern;

    void fireLasers(AttackPattern pattern);
};

#endif //VAPE_BOSS2_HPP
