//
// Created by Cody on 9/2/2019.
//

#ifndef INC_436D_BULLET_H
#define INC_436D_BULLET_H


#include "common.hpp"
#include "turtle.hpp"
#include "fish.hpp"

class Bullet : public Renderable {
    // Shared between all bullets, no need to load one for each instance
    static Texture bullet_texture;

public:
    bool init(vec2 position, float rotation);
    void destroy();
    void update(float ms);
    void draw(const mat3& projection)override;

    // Collision routines for turtles and fish
    bool collides_with(const Turtle& turtle);
    bool collides_with(const Fish& fish);

private:
    vec2 m_position; // Window coordinates
    vec2 m_scale; // 1.f in each dimension. 1.f is as big as the associated texture
    float m_rotation; // in radians
};


#endif //INC_436D_BULLET_H
