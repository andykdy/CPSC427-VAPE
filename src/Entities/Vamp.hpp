//
// Created by matte on 2019-10-04.
//

#ifndef VAPE_VAMP_HPP
#define VAPE_VAMP_HPP

#include "common.hpp"
#include "Entities/Enemies/Enemy.hpp"
#include <Engine/ECS/Entity.hpp>
#include "Entities/Player.hpp"

class Enemy;
class Player;

class Vamp : public EntityOld {
    static Texture vamp_texture;

public:
    bool init(vec2 position);
    void destroy();
    void update(float ms, Player* player);
    void draw(const mat3& projection)override;

    // Returns the current vamp position (centered)
    vec2 get_position()const;

    // Collision routines for turtles and fish
    bool collides_with(const Enemy& enemy);

    // Returns the vamp mode's bounding box for collision detection, called by collides_with()
    vec2 get_bounding_box()const;

    void set_size(int mode);

private:
    vec2 m_position; // Window coordinates
    vec2 m_scale; // 1.f in each dimension. 1.f is as big as the associated texture
    float m_rotation; // in radians
};


#endif //VAPE_VAMP_HPP
