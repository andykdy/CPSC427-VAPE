//
// Created by Cody on 9/2/2019.
//

#include <cmath>
#include <algorithm>
#include <Components/SpriteComponent.hpp>
#include <Components/PhysicsComponent.hpp>
#include <Components/EffectComponent.hpp>
#include <Components/TransformComponent.hpp>
#include <Components/MotionComponent.hpp>
#include "bullet.hpp"
#include "Entities/Player.hpp"
#include "Entities/Bosses/Boss1.hpp"

Texture Bullet::bullet_texture;

float BULLET_SPEED = 1250;

bool Bullet::init(vec2 position, float rotation) {
    gl_flush_errors();
    auto* sprite = addComponent<SpriteComponent>();
    auto* effect = addComponent<EffectComponent>();
    auto* physics = addComponent<PhysicsComponent>();
    auto* motion = addComponent<MotionComponent>();
    auto* transform = addComponent<TransformComponent>();

    // Load shared texture
    if (!bullet_texture.is_valid())
    {
        if (!bullet_texture.load_from_file(textures_path("bullet.png")))
        {
            fprintf(stderr, "Failed to load bullet texture!");
            return false;
        }
    }

    // Loading shaders
    if (!effect->load_from_file(shader_path("textured.vs.glsl"), shader_path("textured.fs.glsl")))
        return false;

    if (!sprite->initTexture(&bullet_texture))
        throw std::runtime_error("Failed to initialize bullet sprite");

    if (gl_has_errors())
        return false;

    physics->scale = {0.4f, 0.4f};

    motion->radians = rotation;
    // place bullet n away from center of entity
    motion->position.x = position.x + 100*sin(motion->radians);
    motion->position.y = position.y + 100*cos(motion->radians);

    return true;
}

void Bullet::destroy() {
    auto* effect = getComponent<EffectComponent>();
    auto* sprite = getComponent<SpriteComponent>();

    effect->release();
    sprite->release();
    ECS::Entity::destroy();
}

void Bullet::update(float ms) {
    auto* motion = getComponent<MotionComponent>();
    float step = BULLET_SPEED * (ms / 1000);
    motion->position.x += step*sin(motion->radians);
    motion->position.y += step*cos(motion->radians);
}

void Bullet::draw(const mat3 &projection) {

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

vec2 Bullet::get_position()const
{
    auto* motion = getComponent<MotionComponent>();
    return motion->position;
}

// TODO make a collision component and use a single collides_with over Entity to reduce duplicate code
bool Bullet::collides_with(const Player &player) {
    auto* motion = getComponent<MotionComponent>();
    auto* physics = getComponent<PhysicsComponent>();

    float dx = motion->position.x - player.get_position().x;
    float dy = motion->position.y - player.get_position().y;
    float d_sq = dx * dx + dy * dy;
    float other_r = std::max(player.get_bounding_box().x, player.get_bounding_box().y);
    float my_r = std::max(physics->scale.x, physics->scale.y);
    float r = std::max(other_r, my_r);
    r *= 0.6f;
    if (d_sq < r * r)
        return true;
    return false;
}

bool Bullet::collides_with(const Entity &turtle) {
    auto* motion = getComponent<MotionComponent>();
    auto* physics = getComponent<PhysicsComponent>();

    float dx = motion->position.x - turtle.get_position().x;
    float dy = motion->position.y - turtle.get_position().y;
    float d_sq = dx * dx + dy * dy;
    float other_r = std::max(turtle.get_bounding_box().x, turtle.get_bounding_box().y);
    float my_r = std::max(physics->scale.x, physics->scale.y);
    float r = std::max(other_r, my_r);
    r *= 0.6f;
    if (d_sq < r * r)
        return true;
    return false;
}

bool Bullet::collides_with(const Fish &fish) {
    auto* motion = getComponent<MotionComponent>();
    auto* physics = getComponent<PhysicsComponent>();

    float dx = motion->position.x - fish.get_position().x;
    float dy = motion->position.y - fish.get_position().y;
    float d_sq = dx * dx + dy * dy;
    float other_r = std::max(fish.get_bounding_box().x, fish.get_bounding_box().y);
    float my_r = std::max(physics->scale.x, physics->scale.y);
    float r = std::max(other_r, my_r);
    r *= 0.6f;
    if (d_sq < r * r)
        return true;
    return false;
}

bool Bullet::collides_with(const Boss &boss) {
    auto* motion = getComponent<MotionComponent>();
    auto* physics = getComponent<PhysicsComponent>();

    float dx = motion->position.x - boss.get_position().x;
    float dy = motion->position.y - boss.get_position().y;
    float d_sq = dx * dx + dy * dy;
    float other_r = std::max(boss.get_bounding_box().x, boss.get_bounding_box().y);
    float my_r = std::max(physics->scale.x, physics->scale.y);
    float r = std::max(other_r, my_r);
    r *= 0.6f;
    if (d_sq < r * r)
        return true;
    return false;
}

// Returns the local bounding coordinates scaled by the current size of the turtle
vec2 Bullet::get_bounding_box()const
{
    auto* physics = getComponent<PhysicsComponent>();
    // fabs is to avoid negative scale due to the facing direction
    return { std::fabs(physics->scale.x) * bullet_texture.width, std::fabs(physics->scale.y) * bullet_texture.height };
}

bool Bullet::isOffScreen(const vec2 &screen) {
    float h = get_bounding_box().y / 2;
    return (get_position().y - h > screen.y);
}
