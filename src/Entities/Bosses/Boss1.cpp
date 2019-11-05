//
// Created by cody on 9/26/19.
//

#include "Boss1.hpp"
#include <cmath>
#include <iostream>

// Same as static in c, local to compilation unit
namespace
{
    const size_t BULLET_COOLDOWN_MS = 100;
    const size_t INIT_HEALTH = 100;
}

Texture Boss1::boss1_texture;

bool Boss1::init() {
    // Load shared texture
    if (!boss1_texture.is_valid())
    {
        if (!boss1_texture.load_from_file(textures_path("boss1.png")))
        {
            fprintf(stderr, "Failed to load Boss1 texture!");
            return false;
        }
    }

    // The position corresponds to the center of the texture
    float wr = boss1_texture.width * 0.5f;
    float hr = boss1_texture.height * 0.5f;

    TexturedVertex vertices[4];
    vertices[0].position = { -wr, +hr, -0.02f };
    vertices[0].texcoord = { 0.f, 1.f };
    vertices[1].position = { +wr, +hr, -0.02f };
    vertices[1].texcoord = { 1.f, 1.f };
    vertices[2].position = { +wr, -hr, -0.02f };
    vertices[2].texcoord = { 1.f, 0.f };
    vertices[3].position = { -wr, -hr, -0.02f };
    vertices[3].texcoord = { 0.f, 0.f };

    // Counterclockwise as it's the default opengl front winding direction
    uint16_t indices[] = { 0, 3, 1, 1, 3, 2 };

    // Clearing errors
    gl_flush_errors();

    // Vertex Buffer creation
    glGenBuffers(1, &mesh.vbo);
    glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(TexturedVertex) * 4, vertices, GL_STATIC_DRAW);

    // Index Buffer creation
    glGenBuffers(1, &mesh.ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint16_t) * 6, indices, GL_STATIC_DRAW);

    // Vertex Array (Container for Vertex + Index buffer)
    glGenVertexArrays(1, &mesh.vao);
    if (gl_has_errors())
        return false;

    // Loading shaders
    if (!effect.load_from_file(shader_path("textured.vs.glsl"), shader_path("textured.fs.glsl")))
        return false;

    motion.position = {0.f, 0.f };
    motion.radians = 3.14;
    motion.speed = 350.f;

    // Setting initial values, scale is negative to make it face the opposite way
    // 1.0 would be as big as the original texture.
    physics.scale = { -0.4f, 0.4f };

    health = INIT_HEALTH;
    m_is_alive = true;

    int randomVal = rand() % 2;
    dir = (randomVal == 0 ? Direction::right : Direction::left);

    return true;
}

void Boss1::destroy() {
    for (auto& bullet : bullets)
        bullet.destroy();
    bullets.clear();

    glDeleteBuffers(1, &mesh.vbo);
    glDeleteBuffers(1, &mesh.ibo);
    glDeleteBuffers(1, &mesh.vao);

    glDeleteShader(effect.vertex);
    glDeleteShader(effect.fragment);
    glDeleteShader(effect.program);
}

void Boss1::update(float ms) {
    // Update bullets
    for (auto& bullet : bullets)
        bullet.update(ms);

    // Simple health based states, only two states for this first boss
    if (health > 50) state1Update(ms);
    else if (health > 0) state2Update(ms);
    else {
        // TODO death
    }
}

void Boss1::state1Update(float ms) {
    int w, h;
    glfwGetFramebufferSize(GameEngine::getInstance().getM_window(), &w, &h);
    vec2 screen = { (float)w / GameEngine::getInstance().getM_screen_scale(), (float)h / GameEngine::getInstance().getM_screen_scale() };
    // Set left and right boundaries such that ship doesn't leave the screen
    float lBound = (float)boss1_texture.width * 0.5f;
    float rBound = (float)screen.x - lBound;
    // Adjust x position by speed in direction dir
    int mod = (dir == Direction::right ? 1 : -1);
    float newX = motion.position.x + (float)mod * (ms / 1000) * motion.speed;
    // If past a boundary, set position to within the boundary and swap direction
    if (newX >= rBound) {
        newX = rBound;
        dir = Direction::left;
    } else if (newX <= lBound) {
        newX = lBound;
        dir = Direction::right;
    }
    motion.position.x = newX;

    // shoot
    m_bullet_cooldown -= ms;
    if (m_bullet_cooldown < 0.f) {
        int randomVal = rand() % 20;
        if (randomVal < 2) {
            m_bullet_cooldown = BULLET_COOLDOWN_MS;
            spawnBullet();
            // TODO play sound
        }
    }
}

void Boss1::state2Update(float ms) {
    // Randomly switch directions
    int randomVal = rand() % 100;
    if (randomVal < 1) {
        dir = (dir == Direction::left ? Direction::right : Direction::left);
    }
    state1Update(ms);
}


void Boss1::draw(const mat3 &projection) {
    // Draw boss' bullets
    for (auto& bullet : bullets)
        bullet.draw(projection);

    // Transformation code, see Rendering and Transformation in the template specification for more info
    // Incrementally updates transformation matrix, thus ORDER IS IMPORTANT
    transform.begin();
    transform.translate(motion.position);
    transform.rotate(motion.radians);
    transform.scale(physics.scale);
    transform.end();

    // Setting shaders
    glUseProgram(effect.program);

    // Enabling alpha channel for textures
    glEnable(GL_BLEND); glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDisable(GL_DEPTH_TEST);

    // Getting uniform locations for glUniform* calls
    GLint transform_uloc = glGetUniformLocation(effect.program, "transform");
    GLint color_uloc = glGetUniformLocation(effect.program, "fcolor");
    GLint projection_uloc = glGetUniformLocation(effect.program, "projection");

    // Setting vertices and indices
    glBindVertexArray(mesh.vao);
    glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.ibo);

    // Input data location as in the vertex buffer
    GLint in_position_loc = glGetAttribLocation(effect.program, "in_position");
    GLint in_texcoord_loc = glGetAttribLocation(effect.program, "in_texcoord");
    glEnableVertexAttribArray(in_position_loc);
    glEnableVertexAttribArray(in_texcoord_loc);
    glVertexAttribPointer(in_position_loc, 3, GL_FLOAT, GL_FALSE, sizeof(TexturedVertex), (void*)0);
    glVertexAttribPointer(in_texcoord_loc, 2, GL_FLOAT, GL_FALSE, sizeof(TexturedVertex), (void*)sizeof(vec3));

    // Enabling and binding texture to slot 0
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, boss1_texture.id);

    // Setting uniform values to the currently bound program
    glUniformMatrix3fv(transform_uloc, 1, GL_FALSE, (float*)&transform.out);
    float color[] = { 1.f, 1.f, 1.f };
    glUniform3fv(color_uloc, 1, color);
    glUniformMatrix3fv(projection_uloc, 1, GL_FALSE, (float*)&projection);

    // Drawing!
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, nullptr);
}

vec2 Boss1::get_position() const {
    return motion.position;
}

void Boss1::set_position(vec2 position) {
    motion.position = position;
}

vec2 Boss1::get_bounding_box() const {
    // Returns the local bounding coordinates scaled by the current size of the turtle
    // fabs is to avoid negative scale due to the facing direction.
    return { std::fabs(physics.scale.x) * boss1_texture.width, std::fabs(physics.scale.y) * boss1_texture.height };
}

void Boss1::spawnBullet() {
    Bullet bullet;
    if (bullet.init(motion.position, motion.radians+ 3.14)) {
        bullets.emplace_back(bullet);
    } else {
        throw std::runtime_error("Failed to spawn bullet");
    }
}

int Boss1::getHealth() const {
    return health;
}

void Boss1::addDamage(int damage) {
    health -= damage;
}

bool Boss1::is_alive() {
    return m_is_alive;
}

void Boss1::kill() {
    m_is_alive = false;
}
