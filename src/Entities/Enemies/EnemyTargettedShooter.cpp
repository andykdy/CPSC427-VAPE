//
// Created by Tanha Kabir on 2019-11-11.
//

#include "EnemyTargettedShooter.hpp"

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

Texture EnemyTargettedShooter::texture;
using namespace std;

namespace
{
    const size_t POINTS_VAL = 500;
    const size_t BURST_COOLDOWN_MS = 600;
    const size_t BULLET_COOLDOWN_MS = 200;
    const size_t PAUSE_TIME_MS = 4000;
    const size_t BULLET_DAMAGE = 5;
}

bool EnemyTargettedShooter::init() {
    auto* sprite = addComponent<SpriteComponent>();
    auto* effect = addComponent<EffectComponent>();
    auto* physics = addComponent<PhysicsComponent>();
    auto* motion = addComponent<MotionComponent>();
    auto* transform = addComponent<TransformComponent>();

    // Load shared texture
    if (!texture.is_valid())
    {
        if (!texture.load_from_file(textures_path("turtle3.png")))
        {
            fprintf(stderr, "Failed to load turtle texture!");
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

    motion->radians = M_PI;
    motion->velocity = {0.f, 180.f};

    // Setting initial values, scale is negative to make it face the opposite way
    // 1.0 would be as big as the original texture.
    physics->scale = { -0.28f, 0.28f };

    m_burst_count = 0;
    m_burst_cooldown = BURST_COOLDOWN_MS;
    m_bullet_cooldown = 0;
    points = POINTS_VAL;

    return true;
}

void EnemyTargettedShooter::destroy() {
    auto* effect = getComponent<EffectComponent>();
    auto* sprite = getComponent<SpriteComponent>();

    effect->release();
    sprite->release();
    ECS::Entity::destroy();
}

void EnemyTargettedShooter::update(float ms) {
    auto* motion = getComponent<MotionComponent>();
    bool stationed_to_shoot = motion->velocity.x == 0 && motion->velocity.y == 0 &&
                                 motion->position.x > 0 && motion->position.y > 0;

    if (m_pause_cooldown_ms > 0) {
        m_pause_cooldown_ms -= ms;
        motion->velocity = {0,0};
        m_was_stationed = true;
    } else {
        if(motion->position.y > 150 && !m_was_stationed) {
            m_pause_cooldown_ms = PAUSE_TIME_MS;
        } else if (motion->position.y < -150 && !m_was_stationed) {
            m_pause_cooldown_ms = PAUSE_TIME_MS;
        } else {
            motion->velocity.y = 180.f;
        }
    }

    if (stationed_to_shoot) {
        float dy = player_position.y - motion->position.y;
        float dx = player_position.x - motion->position.x;

        float angle = atan2(dx, dy);

        motion->radians = angle - M_PI;
    } else {
        motion->radians = M_PI;
    }

    // shoot
    if (m_burst_cooldown > 0) {
        m_burst_cooldown -= ms;
    } else {
        if (m_burst_count >= 1) {
            m_burst_count = 0;
            m_burst_cooldown = BURST_COOLDOWN_MS;
        } else if (m_bullet_cooldown > 0) {
            m_bullet_cooldown -= ms;
        } else {
            ++m_burst_count;
            m_bullet_cooldown = BULLET_COOLDOWN_MS;

            if(stationed_to_shoot) {
                spawnBullet();
            }
        }
    }
}

void EnemyTargettedShooter::draw(const mat3 &projection) {
    auto* transform = getComponent<TransformComponent>();
    auto* effect = getComponent<EffectComponent>();
    auto* motion = getComponent<MotionComponent>();
    auto* physics = getComponent<PhysicsComponent>();
    auto* sprite = getComponent<SpriteComponent>();
    auto* enemy = addComponent<EnemyComponent>();

    transform->begin();
    transform->translate(motion->position);
    transform->scale(physics->scale);
    transform->rotate(motion->radians);
    transform->end();

    sprite->draw(projection, transform->out, effect->program);
}

vec2 EnemyTargettedShooter::get_position() const {
    auto* motion = getComponent<MotionComponent>();
    return motion->position;
}

void EnemyTargettedShooter::set_position(vec2 position) {
    auto* motion = getComponent<MotionComponent>();
    motion->position = position;
}

vec2 EnemyTargettedShooter::get_bounding_box() const {
    auto* physics = getComponent<PhysicsComponent>();

    // Returns the local bounding coordinates scaled by the current size of the turtle
    // fabs is to avoid negative scale due to the facing direction.
    return { std::fabs(physics->scale.x) * texture.width, std::fabs(physics->scale.y) * texture.height };
}

void EnemyTargettedShooter::set_velocity(vec2 velocity) {
    auto* motion = getComponent<MotionComponent>();
    motion->velocity.x = velocity.x;
    motion->velocity.y = velocity.y;
}

void EnemyTargettedShooter::spawnBullet() {
    auto & projectiles = GameEngine::getInstance().getSystemManager()->getSystem<ProjectileSystem>();
    auto *motion = getComponent<MotionComponent>();
    Bullet *bullet = &GameEngine::getInstance().getEntityManager()->addEntity<Bullet>();
    if (bullet->init(motion->position, motion->radians + M_PI, true, BULLET_DAMAGE)) {
        bullet->set_speed_slow();
        projectiles.hostile_projectiles.emplace_back(bullet);
    } else {
        throw std::runtime_error("Failed to spawn bullet");
    }
}
