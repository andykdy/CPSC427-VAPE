//
// Created by Cody on 11/5/2019.
//

#ifndef VAPE_BOSS_HPP
#define VAPE_BOSS_HPP

#include <common.hpp>
#include <Engine/ECS/Entity.hpp>
#include <Entities/Vamp.hpp>
#include "../Projectiles and Damaging/bullet.hpp"
#include "../UI/BossHealth.hpp"

class Projectile;

enum collisionType {
    radius,
    exact
};

class Boss : public ECS::Entity { // TODO refactor to new entity
    // Shared between all turtles, no need to load one for each instance
    static Texture boss_texture;
protected:
    BossHealth* m_healthbar;
    int health = 1;
    bool m_is_alive = true;
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

    virtual bool collidesWith(Vamp vamp) = 0;
};

#endif //VAPE_BOSS_HPP
