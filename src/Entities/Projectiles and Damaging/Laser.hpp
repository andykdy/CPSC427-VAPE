//
// Created by Cody on 11/7/2019.
//

#ifndef VAPE_LASER_HPP
#define VAPE_LASER_HPP

#include "Projectile.hpp"

enum laserState {
    off,
    primed,
    firing,
};

// TODO laser sweeping;

class Laser : public Projectile {
    struct Particle {
        float life = 0.0f; // remove pebble when its life reaches 0
        vec2 position{0,0};
        vec2 velocity{0,0};
        vec4 color {1.f,1.f,1.f,1.f};
    };
public:
    bool init(vec2 position, float rotation) override;
    void destroy() override;
    void update(float ms) override;
    void draw(const mat3& projection) override;

    vec2 get_position() const override;
    void set_position(vec2 position) { m_origin = position; }

    bool collides_with(const Player &player) override;
    bool collides_with(const Enemy &turtle) override;
    bool collides_with(const Boss &boss) override; // TODO player lasers?

    bool isOffScreen(const vec2 &screen) override;

    inline void setState(laserState state) { m_state = state; };
private:
    vec2 m_origin;
    float m_rotation;
    laserState m_state;
    std::vector<Particle> m_particles;
    GLuint m_vbo;
    GLuint m_instance_vbo;

    void spawn();
};


#endif //VAPE_LASER_HPP
