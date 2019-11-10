//
// Created by Cody on 11/7/2019.
//

#ifndef VAPE_LASER_HPP
#define VAPE_LASER_HPP


#include "Projectile.hpp"

class Laser : public Projectile {
public:
    bool init(vec2 position, float rotation) override {
        return false;
    }

    void destroy() override {
        Entity::destroy();
    }

    void update(float ms) override {
        Entity::update(ms);
    }

    void draw(const mat3& projection) override {
        Entity::draw(projection);
    }

    // Returns the current bullet position
    vec2 get_position()const override {
        return vec2();
    }

    // Collision routines for player, turtles and fish
    bool collides_with(const Player& player) override {
        return false;
    }

    bool collides_with(const Turtle& turtle) override { return false; };
    bool collides_with(const Fish& fish){ return false; };
    bool collides_with(const Boss& boss) override { return false; }; // TODO player lasers?

    // Returns the bullet' bounding box for collision detection, called by collides_with()
    vec2 get_bounding_box()const {
        return vec2();
    }

    bool isOffScreen(const vec2 &screen) override { return false; };
};


#endif //VAPE_LASER_HPP
