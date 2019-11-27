//
// Created by Cody on 11/26/2019.
//

#ifndef VAPE_BOSS3_HPP
#define VAPE_BOSS3_HPP


#include <common.hpp>
#include "Boss.hpp"

class Boss3 : public Boss {
    static Texture boss3_texture;
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

    bool collidesWith(const Vamp& vamp) override;

    bool collidesWith(const Player &player) override;

    bool checkCollision(vec2 pos, vec2 box) const override;

private:
};


#endif //VAPE_BOSS3_HPP