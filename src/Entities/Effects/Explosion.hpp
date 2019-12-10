#pragma once

#include <vector>

#include "common.hpp"

class Explosion : public EntityOld
{
public:
    // Data structure for pebble contains information needed
    // to render and simulate a basic pebble (apart from mesh.vbo),
    // we will use this layout to pass information from m_pebbles to the pipeline.
    struct Particle {
        float life = 0.0f; // remove pebble when its life reaches 0
        vec2 position;
        vec2 velocity;
        float radius;
    };

    // Creates all the associated render resources
    bool init();

    // Releases all associated resources
    void destroy();

    // Updates all pebbles
    // ms represents the number of milliseconds elapsed from the previous update() call
    void update(float ms);

    // Renders the pebbles
    // projection is the 2D orthographic projection matrix
    void draw(const mat3& projection) override;

    // Spawn new pebble
    void spawn(vec2 position);

    void spawnBossExplosion(vec2 position, vec2 box);


private:
    GLuint m_instance_vbo; // vbo for instancing particles
    std::vector<Particle> m_particles; // vector of particles
};