//
// Created by matte on 9/2/2019.
//

#include <cmath>
#include <algorithm>
#include "Vamp.hpp"
#include <Engine/ECS/Entity.hpp>
#include <Systems/PickupSystem.hpp>

Texture Vamp::vamp_texture;


bool Vamp::init(vec2 position) {
    // Load shared texture
    if (!vamp_texture.is_valid())
    {
        if (!vamp_texture.load_from_file(textures_path("vamp_mode.png")))
        {
            fprintf(stderr, "Failed to load vamp texture!");
            return false;
        }
    }

    // The position corresponds to the center of the texture
    set_size(1);

    // counterclockwise as it's the default opengl front winding direction
    uint16_t indices[] = {0, 3, 1, 1, 3, 2};

    // Clearing errors
    gl_flush_errors();

    // Index Buffer creation
    glGenBuffers(1, &mesh.ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint16_t) * 6, indices, GL_STATIC_DRAW);

    // Vertex Array (Container for Vertex + Index buffer)
    glGenVertexArrays(1, &mesh.vao);
    if (gl_has_errors())
        return false;

    // Loading shaders
    if (!effect.load_from_file(shader_path("vamp.vs.glsl"), shader_path("vamp.fs.glsl")))
        return false;

    motion.radians = 3.14f;
    motion.speed = 360.f;

    m_position.x = position.x;
    m_position.y = position.y;

    return true;
}

void Vamp::set_size(int mode) {
    TexturedVertex vertices[4];
    if (mode == 1) {
        float wr = vamp_texture.width * 0.35f;
        float hr = vamp_texture.height * 0.35f;

        vertices[0].position = {-wr, +hr, -0.05f};
        vertices[0].texcoord = {0.f, 1.f};
        vertices[1].position = {+wr, +hr, -0.05f};
        vertices[1].texcoord = {1.f, 1.f};
        vertices[2].position = {+wr, -hr, -0.05f};
        vertices[2].texcoord = {1.f, 0.f};
        vertices[3].position = {-wr, -hr, -0.05f};
        vertices[3].texcoord = {0.f, 0.f};

        m_scale.x = 0.6f;
        m_scale.y = 0.6f;
    } else {
        float mult = 1.05f;
        float wr = vamp_texture.width * 0.35f * mult;
        float hr = vamp_texture.height * 0.35f * mult;

        vertices[0].position = {-wr, +hr, -0.05f};
        vertices[0].texcoord = {0.f, 1.f};
        vertices[1].position = {+wr, +hr, -0.05f};
        vertices[1].texcoord = {1.f, 1.f};
        vertices[2].position = {+wr, -hr, -0.05f};
        vertices[2].texcoord = {1.f, 0.f};
        vertices[3].position = {-wr, -hr, -0.05f};
        vertices[3].texcoord = {0.f, 0.f};

        m_scale.x = 0.6f * mult * 2.f;
        m_scale.y = 0.6f * mult * 2.f;
    }
    // Vertex Buffer creation
    glGenBuffers(1, &mesh.vbo);
    glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(TexturedVertex) * 4, vertices, GL_STATIC_DRAW);
}

void Vamp::destroy() {
    glDeleteBuffers(1, &mesh.vbo);
    glDeleteBuffers(1, &mesh.ibo);
    glDeleteVertexArrays(1, &mesh.vao);

    glDeleteShader(effect.vertex);
    glDeleteShader(effect.fragment);
    glDeleteShader(effect.program);
}

void Vamp::update(float ms, Player *player) {
    float step = motion.speed * (ms / 1000);
    m_position.x = player->get_position().x;
    m_position.y = player->get_position().y;
    motion.radians -= step * 0.015;
    if (player->get_vamp_expand()) {
        set_size(2);
        player->set_vamp_expand(false);
    } else {
        set_size(1);
    }
}

void Vamp::draw(const mat3 &projection) {
    // Transformation code, see Rendering and Transformation in the template specification for more info
    // Incrementally updates transformation matrix, thus ORDER IS IMPORTANT
    transform.begin();
    transform.translate(m_position);
    transform.rotate(motion.radians);
    transform.scale(m_scale);
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
    glBindTexture(GL_TEXTURE_2D, vamp_texture.id);

    // Setting uniform values to the currently bound program
    glUniformMatrix3fv(transform_uloc, 1, GL_FALSE, (float*)&transform);
    float color[] = { 1.f, 1.f, 1.f, 0.2f };
    glUniform3fv(color_uloc, 1, color);
    glUniformMatrix3fv(projection_uloc, 1, GL_FALSE, (float*)&projection);

    // Drawing!
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, nullptr);
}

vec2 Vamp::get_position()const
{
    return m_position;
}

bool Vamp::collides_with(const Enemy &enemy) {
    float dx = m_position.x - enemy.get_position().x;
    float dy = m_position.y - enemy.get_position().y;
    float d_sq = dx * dx + dy * dy;
    float other_r = std::max(enemy.get_bounding_box().x, enemy.get_bounding_box().y);
    float my_r = std::max(vamp_texture.width * m_scale.x * 0.55f,  vamp_texture.height * m_scale.y * 0.55f);
    float r = std::max(other_r, my_r);
    r *= 0.65f;
    if (d_sq < r * r)
        return true;
    return false;
}

vec2 Vamp::get_bounding_box()const
{
    return { std::fabs(m_scale.x) * vamp_texture.width, std::fabs(m_scale.y) * vamp_texture.height };
}
