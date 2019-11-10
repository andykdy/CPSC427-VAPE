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
        throw std::runtime_error("Failed to initialize bullet sprite");

    if (gl_has_errors())
        return false;

    // TODO mesh for complex collisions

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

    m_healthbar->draw(projection);
}

vec2 Boss2::get_position() const {
    auto* motion = getComponent<MotionComponent>();
    return motion->position;
}

void Boss2::set_position(vec2 position) {
    auto* motion = getComponent<MotionComponent>();
    motion->position = position;
}

vec2 Boss2::get_bounding_box() const {
    auto* physics = getComponent<PhysicsComponent>();
    // Returns the local bounding coordinates scaled by the current size of the turtle
    // fabs is to avoid negative scale due to the facing direction.
    return { std::fabs(physics->scale.x) * SPRITE_W, std::fabs(physics->scale.y) * SPRITE_H };
}
