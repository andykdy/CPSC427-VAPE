//
// Created by Cody on 11/5/2019.
//

#ifndef VAPE_BOSS_HPP
#define VAPE_BOSS_HPP

#include <common.hpp>
#include <Engine/ECS/Entity.hpp>
#include <Entities/Vamp.hpp>
#include "../Projectiles and Damaging/Projectile.hpp"
#include "Entities/UI/BossHealth/BossHealth.hpp"

// Forward Declarations
class Projectile;
class Player;
class Vamp;

enum collisionType {
    radius,
    exact
};

class Boss : public ECS::Entity {
    // Shared between all turtles, no need to load one for each instance
    static Texture boss_texture;
protected:
    BossHealth* m_healthbar;
    int health = 1;
    bool m_is_alive = true;
    float vamp_timer = 0;
    int points = 0;
public:
    // Creates all the associated render resources and default transform
    virtual bool init(vec2 screen) = 0;

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

    virtual bool checkCollision(vec2 pos, vec2 box) const = 0;

    std::vector<Projectile*> projectiles;

    inline int getHealth() const { return health; };

    virtual inline void addDamage(int damage) { health -= damage; };

    inline bool is_alive() {return m_is_alive;};

    inline void kill() { m_is_alive = false; };

    virtual bool collidesWith(const Vamp& vamp) = 0;

    virtual bool collidesWith(const Player& player) = 0;

    inline void add_vamp_timer(float ms) { vamp_timer += ms; };

    inline void reset_vamp_timer() { vamp_timer = 0; };

    inline float get_vamp_timer() { return vamp_timer; };

    inline int get_points() { return points; };
};

#endif //VAPE_BOSS_HPP
