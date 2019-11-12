//
// Created by Cody on 11/7/2019.
//

#include <Components/SpriteComponent.hpp>
#include <Components/EffectComponent.hpp>
#include <Components/PhysicsComponent.hpp>
#include <Components/TransformComponent.hpp>
#include "Laser.hpp"


constexpr int NUM_SEGMENTS = 12;
constexpr int MAX_PARTICLES = 5000;

bool Laser::init(vec2 position, float rotation) {
    gl_flush_errors();
    auto* effect = addComponent<EffectComponent>();

    // Loading shaders
    if (!effect->load_from_file(shader_path("laser.vs.glsl"), shader_path("laser.fs.glsl")))
        return false;

    if (gl_has_errors())
        return false;


    std::vector<GLfloat> screen_vertex_buffer_data;
    constexpr float z = -0.1;

    for (int i = 0; i < NUM_SEGMENTS; i++) {
        screen_vertex_buffer_data.push_back(std::cos(M_PI * 2.0 * float(i) / (float)NUM_SEGMENTS));
        screen_vertex_buffer_data.push_back(std::sin(M_PI * 2.0 * float(i) / (float)NUM_SEGMENTS));
        screen_vertex_buffer_data.push_back(z);

        screen_vertex_buffer_data.push_back(std::cos(M_PI * 2.0 * float(i + 1) / (float)NUM_SEGMENTS));
        screen_vertex_buffer_data.push_back(std::sin(M_PI * 2.0 * float(i + 1) / (float)NUM_SEGMENTS));
        screen_vertex_buffer_data.push_back(z);

        screen_vertex_buffer_data.push_back(0);
        screen_vertex_buffer_data.push_back(0);
        screen_vertex_buffer_data.push_back(z);
    }

    // Clearing errors
    gl_flush_errors();

    // Vertex Buffer creation
    glGenBuffers(1, &m_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, screen_vertex_buffer_data.size()*sizeof(GLfloat), screen_vertex_buffer_data.data(), GL_STATIC_DRAW);

    glGenBuffers(1, &m_instance_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_instance_vbo);

    if (gl_has_errors())
        return false;

    // TODO do a texture too


    m_origin = position;
    m_erase_on_collide = false;
    return false;
}

void Laser::destroy() {
    auto* effect = getComponent<EffectComponent>();

    effect->release();
    ECS::Entity::destroy();
}

void Laser::update(float ms) {
    // TODO
    spawn();

    for (auto& particle : m_particles) {
        particle.life -= ms;
        particle.position.x += particle.velocity.x * (ms / 1000);
        particle.position.y += particle.velocity.y * (ms / 1000);
    }

    auto particle_it = m_particles.begin();
    while (particle_it != m_particles.end()) {
        if ((*particle_it).life <= 0 || (*particle_it).position.y > 1000) { // TODO get screen size
            particle_it = m_particles.erase(particle_it);
            continue;
        } else {
            ++particle_it;
        }
    }
}

void Laser::spawn() {
    int n = 0;
    vec2 vel = {0,2000};
    float xmin = m_origin.x - 10;
    float xmax = m_origin.x + 10;
    float ymin = m_origin.y - 10;
    float ymax = m_origin.y + 10;

    if (m_state == laserState::firing) {
        n = 50;
    } else if (m_state == laserState::primed) {
        n = 10;
        xmin = m_origin.x - 1;
        xmax = m_origin.x + 1;
    }

    for (int i = 0; i < n && m_particles.size() < MAX_PARTICLES; i++) {
        m_particles.emplace_back();
        Particle& p = m_particles.back();

        float randx = rand() % (int)(xmax-xmin) + (int)xmin;
        float randy = rand() % (int)(ymax-ymin) + (int)ymin;
        p.position = {randx, randy};
        p.velocity = vel;
        p.life = 2000;
        p.color = {1.f,1.f,1.f,1.f};
        if (m_state == laserState::firing) {
            float mod = (std::fabs(randx - m_origin.x) / 10);
            p.color = {1.f * mod, 1.f * mod, 1.f, 1.f};
        }
    }
}

void Laser::draw(const mat3 &projection) {
    auto* effect = getComponent<EffectComponent>();

    // Setting shaders
    glUseProgram(effect->program);

    // Enabling alpha channel for textures
    glEnable(GL_BLEND); glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_DEPTH_TEST);

    // Getting uniform locations
    GLint projection_uloc = glGetUniformLocation(effect->program, "projection");
    GLint color_uloc = glGetUniformLocation(effect->program, "color");
    GLint center_uloc = glGetUniformLocation(effect->program, "laser_center");

    float color[] = { 1.f, 1.f, 1.f };
    glUniform3fv(color_uloc, 1, color);
    float center[] = {m_origin.x, m_origin.y};
    glUniform2fv(center_uloc, 1, center);
    glUniformMatrix3fv(projection_uloc, 1, GL_FALSE, (float*)&projection);

    // Draw the screen texture on the geometry
    // Setting vertices
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

    // Mesh vertex positions
    // Bind to attribute 0 (in_position) as in the vertex shader
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glVertexAttribDivisor(0, 0);

    // Load up particles into buffer
    glBindBuffer(GL_ARRAY_BUFFER, m_instance_vbo);
    glBufferData(GL_ARRAY_BUFFER, m_particles.size() * sizeof(Particle), m_particles.data(), GL_DYNAMIC_DRAW);

    // particles translations
    // Bind to attribute 1 (in_translate) as in vertex shader
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Particle), (GLvoid*)offsetof(Particle, position));
    glVertexAttribDivisor(1, 1);

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(Particle), (GLvoid*)offsetof(Particle, color));
    glVertexAttribDivisor(2, 1);

    // Draw using instancing
    // https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glDrawArraysInstanced.xhtml
    glDrawArraysInstanced(GL_TRIANGLES, 0, NUM_SEGMENTS*3, m_particles.size());

    // Reset divisor
    glVertexAttribDivisor(1, 0);
    glVertexAttribDivisor(2, 0);
}

vec2 Laser::get_position() const {
    return m_origin; // TODO?
}

bool Laser::collides_with(const Player &player) {
    if (m_state != laserState::firing) return false;
    // TODO, collision area = (laserXmin, origin.y), (laserXmax, screen.y)
    return false;
}

bool Laser::collides_with(const Enemy &turtle) {
    return false;
}

bool Laser::collides_with(const Boss &boss) {
    return false;
}

bool Laser::isOffScreen(const vec2 &screen) {
    return false;
}
