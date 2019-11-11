//
// Created by Cody on 11/10/2019.
//

#include <Components/SpriteComponent.hpp>
#include <Components/EffectComponent.hpp>
#include <Components/PhysicsComponent.hpp>
#include <Components/MotionComponent.hpp>
#include <Components/TransformComponent.hpp>
#include "HealthIcon.hpp"

Texture HealthIcon::health_icon_texture;

bool HealthIcon::init(vec2 pos) {
    auto* sprite = addComponent<SpriteComponent>();
    auto* effect = addComponent<EffectComponent>();
    auto* physics = addComponent<PhysicsComponent>();
    auto* motion = addComponent<MotionComponent>();
    auto* transform = addComponent<TransformComponent>();

    // Load shared texture
    if (!health_icon_texture.is_valid())
    {
        if (!health_icon_texture.load_from_file(textures_path("health_icon.png")))
        {
            throw std::runtime_error("Failed to load health texture");
        }
    }

    // Loading shaders
    if (!effect->load_from_file(shader_path("textured.vs.glsl"), shader_path("textured.fs.glsl")))
        throw std::runtime_error("Failed to load texture shaders");

    if (!sprite->initTexture(&health_icon_texture))
        throw std::runtime_error("Failed to initialize health sprite");

    physics->scale = { 1.f, 1.f };
    motion->position = { pos.x, pos.y };
    // TODO?
    return true;
}

void HealthIcon::draw(const mat3 &projection) {
    auto* transform = getComponent<TransformComponent>();
    auto* effect = getComponent<EffectComponent>();
    auto* motion = getComponent<MotionComponent>();
    auto* physics = getComponent<PhysicsComponent>();
    auto* sprite = getComponent<SpriteComponent>();

    transform->begin();
    transform->translate(motion->position);
    transform->scale(physics->scale);
    transform->end();

    sprite->draw(projection, transform->out, effect->program, {1.f, 0.1f, 0.1f});
}

void HealthIcon::destroy() {
    auto* effect = getComponent<EffectComponent>();
    auto* sprite = getComponent<SpriteComponent>();

    effect->release();
    sprite->release();

    ECS::Entity::destroy();
}
