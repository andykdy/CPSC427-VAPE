//
// Created by Tanha Kabir on 2019-12-02.
//

#include "EnemyExplosivePayload.hpp"

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

Texture EnemyExplosivePlayload::texture;
using namespace std;

namespace
{
    const size_t POINTS_VAL = 250;
    const size_t BULLET_DAMAGE = 5;
    const size_t ROTATE_COOLDOWN_MS = 100;
    const size_t PAYLOAD_BULLET_COUNT = 10;
    const size_t EXPLOSVE_TIME_MS = 2500;
}

bool EnemyExplosivePlayload::init() {
    auto* sprite = addComponent<SpriteComponent>();
    auto* effect = addComponent<EffectComponent>();
    auto* physics = addComponent<PhysicsComponent>();
    auto* motion = addComponent<MotionComponent>();
    auto* enemy = addComponent<EnemyComponent>();
    auto* transform = addComponent<TransformComponent>();

    // Load shared texture
    if (!texture.is_valid())
    {
        if (!texture.load_from_file(textures_path("turtle4.png")))
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

    motion->radians = M_PI;
    motion->velocity = {0.f, 180.f};

    // Setting initial values, scale is negative to make it face the opposite way
    // 1.0 would be as big as the original texture.
    physics->scale = { -0.23f, 0.23f };

    m_explosive_cooldown_ms = EXPLOSVE_TIME_MS;
    m_rotation_direction = 1.0;
    points = POINTS_VAL;

    return true;
}

void EnemyExplosivePlayload::destroy() {
    if (m_explosive_cooldown_ms > 0.f) {
        spawnPayloadBullets();
    }

    auto* effect = getComponent<EffectComponent>();
    auto* sprite = getComponent<SpriteComponent>();

    effect->release();
    sprite->release();
    ECS::Entity::destroy();
}


void EnemyExplosivePlayload::update(float ms) {
    auto* motion = getComponent<MotionComponent>();

    if (m_rotate_cooldown_ms <= 0.0) {
        m_rotation_direction *= -1.0;
        m_rotate_cooldown_ms = ROTATE_COOLDOWN_MS;
    }

    if (m_explosive_cooldown_ms > 0.f) {
        float step = 180.f * (ms / 1000);
        motion->radians += m_rotation_direction * step * 0.02;

        float dy = player_position.y - motion->position.y;
        float dx = player_position.x - motion->position.x;

        float angle = atan2(dx, dy);

        motion->velocity = {180.f * sin(angle), 180.f * cos(angle)};
    }


    m_rotate_cooldown_ms -= ms;
    m_explosive_cooldown_ms -= ms;
}


void EnemyExplosivePlayload::draw(const mat3 &projection) {
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

vec2 EnemyExplosivePlayload::get_position() const {
    auto* motion = getComponent<MotionComponent>();
    return motion->position;
}

void EnemyExplosivePlayload::set_position(vec2 position) {
    auto* motion = getComponent<MotionComponent>();
    motion->position = position;
}

vec2 EnemyExplosivePlayload::get_bounding_box() const {
    auto* physics = getComponent<PhysicsComponent>();

    // Returns the local bounding coordinates scaled by the current size of the turtle
    // fabs is to avoid negative scale due to the facing direction.
    return { std::fabs(physics->scale.x) * texture.width, std::fabs(physics->scale.y) * texture.height };
}

void EnemyExplosivePlayload::set_velocity(vec2 velocity) {
    auto* motion = getComponent<MotionComponent>();
    motion->velocity.x = velocity.x;
    motion->velocity.y = velocity.y;
}

void EnemyExplosivePlayload::spawnPayloadBullets() {
    auto & projectiles = GameEngine::getInstance().getSystemManager()->getSystem<ProjectileSystem>();
    auto* motion = getComponent<MotionComponent>();

    for(int i = 0; i < PAYLOAD_BULLET_COUNT; i++) {
        Bullet *bullet = &GameEngine::getInstance().getEntityManager()->addEntity<Bullet>();
        if (bullet->init(motion->position, ((3.14f * 2)/PAYLOAD_BULLET_COUNT) * i, true, BULLET_DAMAGE)) {
            projectiles.hostile_projectiles.emplace_back(bullet);
        } else {
            throw std::runtime_error("Failed to spawn bullet");
        }
    }
}
