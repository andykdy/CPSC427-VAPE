//
// Created by Tanha Kabir on 2019-10-03.
//

#include <cmath>
#include <algorithm>
#include <Components/EffectComponent.hpp>
#include <Components/PhysicsComponent.hpp>
#include <Components/MotionComponent.hpp>
#include <Components/TransformComponent.hpp>
#include <Components/SpriteComponent.hpp>
#include "Health.hpp"

Texture Health::health_point_texture;

bool Health::init(vec2 position) {
    auto* sprite = addComponent<SpriteComponent>();
    auto* effect = addComponent<EffectComponent>();
    auto* physics = addComponent<PhysicsComponent>();
    auto* motion = addComponent<MotionComponent>();
    auto* transform = addComponent<TransformComponent>();

    // Load shared texture
    if (!health_point_texture.is_valid())
    {
        if (!health_point_texture.load_from_file(textures_path("health_point.png")))
        {
            throw std::runtime_error("Failed to load health texture");
        }
    }

    // Loading shaders
    if (!effect->load_from_file(shader_path("textured.vs.glsl"), shader_path("textured.fs.glsl")))
        throw std::runtime_error("Failed to load texture shaders");

    if (!sprite->initTexture(&health_point_texture))
        throw std::runtime_error("Failed to initialize health sprite");

    physics->scale = { 0.25f, 0.25f };
    motion->position = { position.x, position.y };

    return true;
}

void Health::update(float ms) {
}

void Health::draw(const mat3 &projection) {
    auto* transform = getComponent<TransformComponent>();
    auto* effect = getComponent<EffectComponent>();
    auto* motion = getComponent<MotionComponent>();
    auto* physics = getComponent<PhysicsComponent>();
    auto* sprite = getComponent<SpriteComponent>();

    // Transformation code, see Rendering and Transformation in the template specification for more info
    // Incrementally updates transformation matrix, thus ORDER IS IMPORTANT
    for (int i = 0; i < health; i++) {
        transform->begin();
        vec2 offset = {i * 5.f, 0.f};
        offset.x += motion->position.x;
        offset.y += motion->position.y;
        transform->translate(offset);
        transform->scale(physics->scale);
        transform->end();

        sprite->draw(projection, transform->out, effect->program);
    }
}

void Health::destroy() {
    auto* effect = getComponent<EffectComponent>();
    auto* sprite = getComponent<SpriteComponent>();

    effect->release();
    sprite->release();
    ECS::Entity::destroy();
}

void Health::setHealth(int health) {
    Health::health = health;
}
