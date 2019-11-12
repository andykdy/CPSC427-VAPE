//
// Created by Cody on 11/7/2019.
//

#include <Engine/GameEngine.hpp>
#include <Components/SpriteComponent.hpp>
#include <Components/EffectComponent.hpp>
#include <Components/PhysicsComponent.hpp>
#include <Components/MotionComponent.hpp>
#include <Components/TransformComponent.hpp>
#include "Boss2.hpp"


// Same as static in c, local to compilation unit
namespace
{
    const size_t INIT_HEALTH = 100;
    const size_t SPRITE_FRAMES = 4;
    const size_t SPRITE_W = 264;
    const size_t SPRITE_H = 88;
}

Texture Boss2::boss2_texture;

bool Boss2::init(vec2 screen) {
    m_healthbar = &GameEngine::getInstance().getEntityManager()->addEntity<BossHealth>();
    m_healthbar->init(screen, INIT_HEALTH);

    m_vertices.clear();

    gl_flush_errors();
    auto* sprite = addComponent<SpriteComponent>();
    auto* effect = addComponent<EffectComponent>();
    auto* physics = addComponent<PhysicsComponent>();
    auto* motion = addComponent<MotionComponent>();
    auto* transform = addComponent<TransformComponent>();

    // Load shared texture
    if (!boss2_texture.is_valid())
    {
        if (!boss2_texture.load_from_file(textures_path("boss2.png")))
        {
            fprintf(stderr, "Failed to load Boss1 texture!");
            return false;
        }
    }

    // Loading shaders
    if (!effect->load_from_file(shader_path("textured.vs.glsl"), shader_path("textured.fs.glsl")))
        return false;

    if (!sprite->initTexture(&boss2_texture, SPRITE_FRAMES, SPRITE_W, SPRITE_H))
        throw std::runtime_error("Failed to initialize boss2 sprite");

    if (gl_has_errors())
        return false;

    // Collision Mesh //TODO mesh scaling?
    FILE* mesh_file = fopen(mesh_path("boss2.testmesh"), "r");
    size_t num_vertices;
    fscanf(mesh_file, "%zu\n", &num_vertices);
    for (size_t i = 0; i < num_vertices; ++i)
    {
        float x, y;
        fscanf(mesh_file, "%f %f\n", &x, &y);
        Vertex vertex;
        vertex.position = { x, y, -0.01f };
        vertex.color = { 1.f,1.f,1.f };
        m_vertices.push_back(vertex);
    }
    m_dot.init();


    motion->position = {0.f, 0.f };
    motion->radians = 0;
    motion->velocity = {0.f, 0.f}; // TODO switch to using velocity / motion system

    // Setting initial values, scale is negative to make it face the opposite way
    // 1.0 would be as big as the original texture.
    physics->scale = { 2.5f, 2.5f };

    health = INIT_HEALTH;
    m_is_alive = true;

    return true;
}

void Boss2::destroy() {
    m_healthbar->destroy();
    m_dot.destroy();

    for (auto laser : projectiles)
        laser->destroy();
    projectiles.clear();

    auto* effect = getComponent<EffectComponent>();
    auto* sprite = getComponent<SpriteComponent>();

    effect->release();
    sprite->release();
    ECS::Entity::destroy();
}

void Boss2::update(float ms) {
    m_healthbar->setHealth(health);

    // Update bullets
    for (auto laser : projectiles)
        laser->update(ms);
}

void Boss2::draw(const mat3 &projection) {
    // Draw boss' bullets
    for (auto laser : projectiles)
        laser->draw(projection);

    auto* transform = getComponent<TransformComponent>();
    auto* effect = getComponent<EffectComponent>();
    auto* motion = getComponent<MotionComponent>();
    auto* physics = getComponent<PhysicsComponent>();
    auto* sprite = getComponent<SpriteComponent>();

    transform->begin();
    transform->translate(motion->position);
    transform->scale(physics->scale);
    transform->rotate(motion->radians);
    transform->end();

    sprite->draw(projection, transform->out, effect->program);

    // Vertex Debug Drawing
    /*
    for (auto& vertex : m_vertices) {
        transform->begin();
        transform->translate(motion->position);
        transform->scale({215.f,215.f});
        transform->rotate(motion->radians + 1.5708f);
        transform->end();

        vec3 pos = mul(transform->out, vec3{vertex.position.x, vertex.position.y, 1.0});
        m_dot.draw(projection, {1.f,1.f,1.f}, {pos.x, pos.y}, 0);
    }
     */

    m_healthbar->draw(projection);
}

vec2 Boss2::get_position() const {
    auto* motion = getComponent<MotionComponent>();
    return motion->position;
}

void Boss2::set_position(vec2 position) {
    auto* motion = getComponent<MotionComponent>();
    motion->position = {position.x, position.y + 25};
}

vec2 Boss2::get_bounding_box() const {
    auto* physics = getComponent<PhysicsComponent>();
    // Returns the local bounding coordinates scaled by the current size of the turtle
    // fabs is to avoid negative scale due to the facing direction.
    return { std::fabs(physics->scale.x) * SPRITE_W, std::fabs(physics->scale.y) * SPRITE_H };
}

void Boss2::addDamage(int damage) {
    // TODO damage indication
    Boss::addDamage(damage);
}

bool Boss2::collidesWith(const Vamp& vamp) {
    return checkCollision(vamp.get_position(), vamp.get_bounding_box());
}

bool Boss2::collidesWith(const Player &player) {
    return checkCollision(player.get_position(), player.get_bounding_box());
}

bool Boss2::checkCollision(vec2 pos, vec2 box) const {
    // TODO replace with complex collision checking
    auto* motion = getComponent<MotionComponent>();
    auto* physics = getComponent<PhysicsComponent>();

    // bounding box slightly larger than sprite
    vec2 bbox = { (std::fabs(physics->scale.x)+0.1f) * boss2_texture.width, (std::fabs(physics->scale.y)+0.1f) * boss2_texture.height };

    // AABB Bounding box check first
    if ( !((fabs(motion->position.x - pos.x) * 2 < (bbox.x + box.x)) &&
         (fabs(motion->position.y - pos.y) * 2 < (bbox.y + box.y)))) {
         return false;
    }

    auto* transform = getComponent<TransformComponent>();
    // For each vertex, check if within the other
    for(auto vertex : m_vertices) {
        transform->begin();
        transform->translate(motion->position);
        transform->scale({215.f,215.f});
        transform->rotate(motion->radians + 1.5708f);
        transform->end();
        vec3 vpos = mul(transform->out, vec3{vertex.position.x, vertex.position.y, 1.0});
        float w = box.x/2;
        float h = box.y/2;
        if (vpos.x >= pos.x - w &&
            vpos.x <= pos.x + w &&
            vpos.y >= pos.y - h &&
            vpos.y <= pos.y + h) {
            return true;
        }
    }
    return false;
}
