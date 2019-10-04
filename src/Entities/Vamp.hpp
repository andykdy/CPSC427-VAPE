//
// Created by matte on 2019-10-04.
//

#ifndef VAPE_VAMP_H
#define VAPE_VAMP_H

#include "common.hpp"
#include "turtle.hpp"
#include "fish.hpp"

class Vamp : public Entity {
    // Shared between all bullets, no need to load one for each instance
    static Texture bullet_texture;

public:
    bool init(vec2 position, float rotation);
    void destroy();
    void update(float ms);
    void draw(const mat3& projection)override;

    // Returns the current bullet position
    vec2 get_position()const;

    // Collision routines for turtles and fish
    bool collides_with(const Turtle& turtle);
    bool collides_with(const Fish& fish);

    // Returns the bullet' bounding box for collision detection, called by collides_with()
    vec2 get_bounding_box()const;

private:
    vec2 m_position; // Window coordinates
    vec2 m_scale; // 1.f in each dimension. 1.f is as big as the associated texture
    float m_rotation; // in radians
};


#endif //VAPE_VAMP_H
