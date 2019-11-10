//
// Created by Cody on 11/9/2019.
//

#include <Components/TransformComponent.hpp>
#include <Components/EffectComponent.hpp>
#include <Components/MeshComponent.hpp>
#include <Engine/GameEngine.hpp>
#include "BossHealth.hpp"

bool BossHealth::init(vec2 screen, int maxHealth) {
    auto* transform = addComponent<TransformComponent>();
    auto* effect = addComponent<EffectComponent>();
    auto* mesh = addComponent<MeshComponent>();

    m_bar = &GameEngine::getInstance().getEntityManager()->addEntity<BossHealthBar>();
    m_bar->init(screen);
    vec2 barsize = m_bar->getSize();
    // TODO: Health bar 'particles' for instanced drawing, or solid bar?
    float w = barsize.x * 0.99f / maxHealth; // width of health bar * 0.95, divided by health
    float h = barsize.y * 0.9f; // TODO, height of health bar * 0.9
    float wr = w * 0.5f;
    float hr = h * 0.5f;

    // Clearing errors
    gl_flush_errors();

    // Allocate Vertex Data Buffer
    // TexturedVertex vertexData[totalSprites * 4];
    glGenBuffers(1, &mesh->vbo);

    // Allocate Index Buffer
    glGenBuffers(1, &mesh->ibo);


    Vertex vertices[4];
    vertices[0].position = {0, +hr, -0.02f};
    vertices[0].color = {1.f, 0.2f, 0.2f};
    vertices[1].position = {w, +hr, -0.02f};
    vertices[1].color = {1.f, 0.2f, 0.2f};
    vertices[2].position = {w, -hr, -0.02f};
    vertices[2].color = {1.f, 0.2f, 0.2f};
    vertices[3].position = {0, -hr, -0.02f};
    vertices[3].color = {1.f, 0.2f, 0.2f};
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
    m_health = maxHealth;

    // Loading shaders
    return effect->load_from_file(shader_path("coloured.vs.glsl"), shader_path("coloured.fs.glsl"));
}

void BossHealth::draw(const mat3 &projection) {
    if (m_bar != nullptr)
        m_bar->draw(projection);

    if (m_health <= 0)
        return;
    auto* transform = getComponent<TransformComponent>();
    auto* effect = getComponent<EffectComponent>();
    auto* mesh = getComponent<MeshComponent>();

    // Transformation code, see Rendering and Transformation in the template specification for more info
    // Incrementally updates transformation matrix, thus ORDER IS IMPORTANT
    transform->begin();
    transform->translate({m_bar->getSize().x*0.005f, m_bar->getSize().y/2});
    transform->rotate(0);
    transform->scale({(float)m_health,1});
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
    float c[] = { 1.f, 0.2f, 0.2f };
    glUniform3fv(color_uloc, 1, c);
    glUniformMatrix3fv(projection_uloc, 1, GL_FALSE, (float*)&projection);

    // Drawing!
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, nullptr);
}

void BossHealth::destroy() {
    if (m_bar != nullptr)
        m_bar->destroy();

    auto* effect = getComponent<EffectComponent>();
    auto* mesh = getComponent<MeshComponent>();
    effect->release();
    mesh->release();
    Entity::destroy();
}

void BossHealth::setHealth(int health) {
    m_health = health;
}
