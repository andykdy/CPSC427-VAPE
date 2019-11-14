//
// Created by matte on 2019-11-12.
//

#pragma once

#include <vector>
#include <random>

#include "common.hpp"


class VampParticleEmitter : public EntityOld {

public:
    struct VampParticle {
        float life = 0.0f;
        vec2 position;
        vec2 velocity;
        float radius;
    };

    // Creates all the associated render resources
    bool init();

    // Releases all associated resources
    void destroy();

    // Updates all vamp particles
    // ms represents the number of milliseconds elapsed from the previous update() call
    void update(float ms, vec2 player_position);

    // Renders the particles
    // projection is the 2D orthographic projection matrix
    void draw(const mat3& projection) override;

    // Spawn new particle
    void spawn(vec2 position);



private:
    GLuint m_instance_vbo; // vbo for instancing particles
    std::vector<VampParticle> m_particles; // vector of particles
    std::uniform_real_distribution<float> m_dist;
    std::default_random_engine m_rng;

};
