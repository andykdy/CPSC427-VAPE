//
// Created by Cody on 11/8/2019.
//

#include <Components/TransformComponent.hpp>
#include <Components/EffectComponent.hpp>
#include <Components/MeshComponent.hpp>
#include "UIPanel.hpp"

bool UIPanel::init(vec2 screen, float height) {
    auto* transform = addComponent<TransformComponent>();
    auto* effect = addComponent<EffectComponent>();
    auto* mesh = addComponent<MeshComponent>();



    // Clearing errors
    gl_flush_errors();

    // Allocate Vertex Data Buffer
//		TexturedVertex vertexData[totalSprites * 4];
    glGenBuffers(1, &mesh->vbo);

    // Allocate Index Buffer
    glGenBuffers(1, &mesh->ibo);


    Vertex vertices[4];
    vertices[0].position = {0, 0, -0.02f};
    vertices[0].color = {0,0,0};
    vertices[1].position = {screen.x, 0, -0.02f};
    vertices[1].color = {0,0,0};
    vertices[2].position = {screen.x, -height, -0.02f};
    vertices[2].color = {0,0,0};
    vertices[3].position = {0, -height, -0.02f};
    vertices[3].color = {0,0,0};
    uint16_t indices[] = { 0, 3, 1, 1, 3, 2 };

    // Clearing errors
    gl_flush_errors();

    // Vertex Buffer creation
    glGenBuffers(1, &mesh->vbo);
    glBindBuffer(GL_ARRAY_BUFFER, mesh->vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * 4, vertices, GL_STATIC_DRAW);

    // Index Buffer creation
    glGenBuffers(1, &mesh->ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint16_t) * 6, indices, GL_STATIC_DRAW);

    // Vertex Array (Container for Vertex + Index buffer)
    glGenVertexArrays(1, &mesh->vao);

    if (gl_has_errors())
        return false;

    m_screen = screen;

    // Loading shaders
    return effect->load_from_file(shader_path("colored.vs.glsl"), shader_path("colored.fs.glsl"));

}

void UIPanel::draw(const mat3 &projection) {
    auto* transform = getComponent<TransformComponent>();
    auto* effect = getComponent<EffectComponent>();
    auto* mesh = getComponent<MeshComponent>();

    // Transformation code, see Rendering and Transformation in the template specification for more info
    // Incrementally updates transformation matrix, thus ORDER IS IMPORTANT
    transform->begin();
    transform->translate({0, m_screen.y});
    transform->rotate(0);
    transform->scale({1,1});
    transform->end();

    // Setting shaders
    glUseProgram(effect->program);

    // Enabling alpha channel for textures
    glEnable(GL_BLEND); glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDisable(GL_DEPTH_TEST);

    // Getting uniform locations for glUniform* calls
    GLint transform_uloc = glGetUniformLocation(effect->program, "transform");
    GLint color_uloc = glGetUniformLocation(effect->program, "color");
    GLint projection_uloc = glGetUniformLocation(effect->program, "projection");

    // Setting vertices and indices
    glBindVertexArray(mesh->vao);
    glBindBuffer(GL_ARRAY_BUFFER, mesh->vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->ibo);

    // Input data location as in the vertex buffer
    GLint in_position_loc = glGetAttribLocation(effect->program, "in_position");
    glEnableVertexAttribArray(in_position_loc);
    glVertexAttribPointer(in_position_loc, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

    // Setting uniform values to the currently bound program
    glUniformMatrix3fv(transform_uloc, 1, GL_FALSE, (float*)&transform->out);
    float c[] = { 0, 0, 0 };
    glUniform3fv(color_uloc, 1, c);
    glUniformMatrix3fv(projection_uloc, 1, GL_FALSE, (float*)&projection);

    // Drawing!
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, nullptr);
}

void UIPanel::destroy() {
    auto* mesh = getComponent<MeshComponent>();
    mesh->release();
    Entity::destroy();
}
