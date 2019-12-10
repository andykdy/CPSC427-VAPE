//
// Created by Cody on 11/16/2019.
//

#ifndef VAPE_LASERBEAMSPRITE_HPP
#define VAPE_LASERBEAMSPRITE_HPP

#include <Engine/ECS/Entity.hpp>

class LaserBeamSprite : public ECS::Entity {
    static Texture laser_texture;
public:
    bool init(vec2 origin);

    void draw(const mat3 &projection, float rotation);

    void destroy() override;

    inline void set_position(const vec2& position) { m_origin = position;};
private:
    vec2 m_origin;
};


#endif //VAPE_LASERBEAMSPRITE_HPP
