//
// Created by Cody on 9/2/2019.
//

#include <cmath>
#include <algorithm>
#include "bullet.hpp"
#include "Player.hpp"
#include "Bosses/Boss1.hpp"

Texture Bullet::bullet_texture;

float BULLET_SPEED = 1250;

bool Bullet::init(vec2 position, float rotation) {
    // Load shared texture
    if (!bullet_texture.is_valid())
    {
        if (!bullet_texture.load_from_file(textures_path("bullet.png")))
        {
            fprintf(stderr, "Failed to load bullet texture!");
            return false;
        }
    }

    // The position corresponds to the center of the texture
    float wr = bullet_texture.width * 0.5f;
    float hr = bullet_texture.height * 0.5f;

    TexturedVertex vertices[4];
    vertices[0].position = { -wr, +hr, -0.02f };
    vertices[0].texcoord = { 0.f, 1.f };
    vertices[1].position = { +wr, +hr, -0.02f };
    vertices[1].texcoord = { 1.f, 1.f };
    vertices[2].position = { +wr, -hr, -0.02f };
    vertices[2].texcoord = { 1.f, 0.f };
    vertices[3].position = { -wr, -hr, -0.02f };
    vertices[3].texcoord = { 0.f, 0.f };

    // counterclockwise as it's the default opengl front winding direction
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


    physics.scale.x = 0.4f;
    physics.scale.y = 0.4f;

    motion.radians = rotation;

    // place bullet n away from center of entity
    motion.position.x = position.x + 100*sin(motion.radians);
    motion.position.y = position.y + 100*cos(motion.radians);

    return true;
}

void Bullet::destroy() {
    glDeleteBuffers(1, &mesh.vbo);
    glDeleteBuffers(1, &mesh.ibo);
    glDeleteBuffers(1, &mesh.vao);

    glDeleteShader(effect.vertex);
    glDeleteShader(effect.fragment);
    glDeleteShader(effect.program);
}

void Bullet::update(float ms) {
    float step = BULLET_SPEED * (ms / 1000);
    motion.position.x += step*sin(motion.radians);
    motion.position.y += step*cos(motion.radians);
}

void Bullet::draw(const mat3 &projection) {
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
    glBindTexture(GL_TEXTURE_2D, bullet_texture.id);

    // Setting uniform values to the currently bound program
    glUniformMatrix3fv(transform_uloc, 1, GL_FALSE, (float*)&transform);
    float color[] = { 1.f, 1.f, 1.f };
    glUniform3fv(color_uloc, 1, color);
    glUniformMatrix3fv(projection_uloc, 1, GL_FALSE, (float*)&projection);

    // Drawing!
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, nullptr);
}

vec2 Bullet::get_position()const
{
    return motion.position;
}

// TODO make a collision component and use a single collides_with over Entity to reduce duplicate code
bool Bullet::collides_with(const Player &player) {
    float dx = motion.position.x - player.get_position().x;
    float dy = motion.position.y - player.get_position().y;
    float d_sq = dx * dx + dy * dy;
    float other_r = std::max(player.get_bounding_box().x, player.get_bounding_box().y);
    float my_r = std::max(physics.scale.x, physics.scale.y);
    float r = std::max(other_r, my_r);
    r *= 0.6f;
    if (d_sq < r * r)
        return true;
    return false;
}

bool Bullet::collides_with(const Turtle &turtle) {
    float dx = motion.position.x - turtle.get_position().x;
    float dy = motion.position.y - turtle.get_position().y;
    float d_sq = dx * dx + dy * dy;
    float other_r = std::max(turtle.get_bounding_box().x, turtle.get_bounding_box().y);
    float my_r = std::max(physics.scale.x, physics.scale.y);
    float r = std::max(other_r, my_r);
    r *= 0.6f;
    if (d_sq < r * r)
        return true;
    return false;
}

bool Bullet::collides_with(const Fish &fish) {
    float dx = motion.position.x - fish.get_position().x;
    float dy = motion.position.y - fish.get_position().y;
    float d_sq = dx * dx + dy * dy;
    float other_r = std::max(fish.get_bounding_box().x, fish.get_bounding_box().y);
    float my_r = std::max(physics.scale.x, physics.scale.y);
    float r = std::max(other_r, my_r);
    r *= 0.6f;
    if (d_sq < r * r)
        return true;
    return false;
}

bool Bullet::collides_with(const Boss1 &boss) {
    float dx = motion.position.x - boss.get_position().x;
    float dy = motion.position.y - boss.get_position().y;
    float d_sq = dx * dx + dy * dy;
    float other_r = std::max(boss.get_bounding_box().x, boss.get_bounding_box().y);
    float my_r = std::max(physics.scale.x, physics.scale.y);
    float r = std::max(other_r, my_r);
    r *= 0.6f;
    if (d_sq < r * r)
        return true;
    return false;
}

// Returns the local bounding coordinates scaled by the current size of the turtle
vec2 Bullet::get_bounding_box()const
{
    // fabs is to avoid negative scale due to the facing direction
    return { std::fabs(physics.scale.x) * bullet_texture.width, std::fabs(physics.scale.y) * bullet_texture.height };
}
