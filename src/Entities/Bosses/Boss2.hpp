//
// Created by Cody on 11/7/2019.
//

#ifndef VAPE_BOSS2_HPP
#define VAPE_BOSS2_HPP


#include "Boss.hpp"

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

    // Renders the Boss
    // projection is the 2D orthographic projection matrix
    void draw(const mat3 &projection) override;

    // Returns the current turtle position
    vec2 get_position() const override;

    // Sets the new Boss position
    void set_position(vec2 position) override;

    // Returns the Boss' bounding box for collision detection, called by collides_with()
    vec2 get_bounding_box() const override;

    bool collidesWith(Vamp vamp) override;

private:

};

#endif //VAPE_BOSS2_HPP
