//
// Created by Tanha Kabir on 2019-12-02.
//

#include "EnemySpeedster.hpp"

#include <cmath>
#include <Components/SpriteComponent.hpp>
#include <Components/EffectComponent.hpp>
#include <Components/PhysicsComponent.hpp>
#include <Components/MotionComponent.hpp>
#include <Components/TransformComponent.hpp>
#include <Entities/Projectiles and Damaging/bullet.hpp>
#include <Engine/GameEngine.hpp>
#include <Components/EnemyComponent.hpp>
#include <Systems/ProjectileSystem.hpp>

Texture EnemySpeedster::texture;
using namespace std;

namespace
{
    const size_t POINTS_VAL = 250;
    const size_t BULLET_DAMAGE = 5;
    const size_t ROTATE_COOLDOWN_MS = 50;
    const size_t STATIONARY_TIME_MS = 3000;
    const size_t MAX_SPEED = 700;
}

bool EnemySpeedster::init() {
    auto* sprite = addComponent<SpriteComponent>();
    auto* effect = addComponent<EffectComponent>();
    auto* physics = addComponent<PhysicsComponent>();
    auto* motion = addComponent<MotionComponent>();
    auto* enemy = addComponent<EnemyComponent>();
    auto* transform = addComponent<TransformComponent>();

    // Load shared texture
    if (!texture.is_valid())
    {
        if (!texture.load_from_file(textures_path("turtle5.png")))
        {
            fprintf(stderr, "Failed to load generic shooter texture!");
            return false;
        }
    }

    // Loading shaders
    if (!effect->load_from_file(shader_path("textured.vs.glsl"), shader_path("textured.fs.glsl")))
        return false;


    if (!sprite->initTexture(&texture))
        throw runtime_error("Failed to initialize health sprite");

    if (gl_has_errors())
        return false;

    motion->radians = M_PI + ((180.f * (ROTATE_COOLDOWN_MS/1000))/2);
    motion->velocity = {0.f, 400.f};

    // Setting initial values, scale is negative to make it face the opposite way
    // 1.0 would be as big as the original texture.
    physics->scale = { -0.35f, 0.35f };

    m_stationary_cooldown_ms = STATIONARY_TIME_MS;
    m_direction = from_top;
    m_rotation_direction = 1.0;
    points = POINTS_VAL;

    return true;
}

void EnemySpeedster::destroy() {
    auto* effect = getComponent<EffectComponent>();
    auto* sprite = getComponent<SpriteComponent>();

    effect->release();
    sprite->release();
    ECS::Entity::destroy();
}

void EnemySpeedster::update(float ms) {
    auto* motion = getComponent<MotionComponent>();

    if (m_rotate_cooldown_ms <= 0.0) {
        m_rotation_direction *= -1.0;
        m_rotate_cooldown_ms = ROTATE_COOLDOWN_MS;
    }

    if (m_stationary_cooldown_ms > 0.f) {
        float step = 180.f * (ms / 1000);
        motion->radians += m_rotation_direction * step * 0.02;

        switch (m_direction) {
            case from_top:
                if(motion->position.y > 70) {
                    motion->velocity = {0.f, 0.f};
                }
                break;
            case from_bottom:
                if(motion->position.y < screen_size.y - 70) {
                    motion->velocity = {0.f, 0.f};
                }
                break;
            case from_left:
                if(motion->position.x > 70) {
                    motion->velocity = {0.f, 0.f};
                }
                break;
            case from_right:
                if(motion->position.x < screen_size.x - 70) {
                    motion->velocity = {0.f, 0.f};
                }
                break;
        }
    } else {
        motion->radians = M_PI;
        motion->velocity = {0.f, MAX_SPEED};

        switch (m_direction) {
            case from_top:
                motion->radians = M_PI;
                motion->velocity = {0.f, MAX_SPEED};
                break;
            case from_bottom:
                motion->radians = 0;
                motion->velocity = {0.f, static_cast<float>(-MAX_SPEED)};
                break;
            case from_left:
                motion->radians = -(M_PI / 2);
                motion->velocity = {MAX_SPEED, 0};
                break;
            case from_right:
                motion->radians = (M_PI / 2);
                motion->velocity = {static_cast<float>(-MAX_SPEED), 0};
                break;
        }
    }


    m_stationary_cooldown_ms -= ms;
    m_rotate_cooldown_ms -= ms;
}

void EnemySpeedster::draw(const mat3 &projection) {
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
}

vec2 EnemySpeedster::get_position() const {
    auto* motion = getComponent<MotionComponent>();
    return motion->position;
}

void EnemySpeedster::set_position(vec2 position) {
    auto* motion = getComponent<MotionComponent>();
    motion->position = position;
}

vec2 EnemySpeedster::get_bounding_box() const {
    auto* physics = getComponent<PhysicsComponent>();

    // Returns the local bounding coordinates scaled by the current size of the turtle
    // fabs is to avoid negative scale due to the facing direction.
    return { std::fabs(physics->scale.x) * texture.width, std::fabs(physics->scale.y) * texture.height };
}

void EnemySpeedster::set_velocity(vec2 velocity) {
    auto* motion = getComponent<MotionComponent>();
    motion->velocity.x = velocity.x;
    motion->velocity.y = velocity.y;

    if(velocity.x == 0.f) {
        if(velocity.y > 0.f) {
            m_direction = from_top;
            motion->radians = M_PI;
        } else {
            m_direction = from_bottom;
            motion->radians = 0;
        }
    } else {
        if(velocity.x > 0.f) {
            m_direction = from_left;
            motion->radians = -(M_PI / 2);
        } else {
            m_direction = from_right;
            motion->radians = (M_PI / 2);
        }
    }
}
