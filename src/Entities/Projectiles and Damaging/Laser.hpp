//
// Created by Cody on 11/7/2019.
//

#ifndef VAPE_LASER_HPP
#define VAPE_LASER_HPP

#include "Projectile.hpp"

class Laser : public Projectile {
public:
    bool init(vec2 position, float rotation) override;
    void destroy() override;
    void update(float ms) override;
    void draw(const mat3& projection) override;

    vec2 get_position() const override;

    bool collides_with(const Player &player) override;
    bool collides_with(const Turtle &turtle) override;
    bool collides_with(const Boss &boss) override; // TODO player lasers?

    bool isOffScreen(const vec2 &screen) override;
};


#endif //VAPE_LASER_HPP
