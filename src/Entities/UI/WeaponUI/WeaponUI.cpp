//
// Created by Andrea Park on 2019-12-05.
//


#include <cmath>
#include <algorithm>
#include <Components/EffectComponent.hpp>
#include <Components/PhysicsComponent.hpp>
#include <Components/MotionComponent.hpp>
#include <Components/TransformComponent.hpp>
#include <Components/SpriteComponent.hpp>
#include <Engine/GameEngine.hpp>
#include "WeaponUI.hpp"

Texture WeaponUI::ui_texture;

bool WeaponUI::init(vec2 position) {
    auto* sprite = addComponent<SpriteComponent>();
    auto* effect = addComponent<EffectComponent>();
    auto* physics = addComponent<PhysicsComponent>();
    auto* motion = addComponent<MotionComponent>();
    auto* transform = addComponent<TransformComponent>();

    // Load shared texture
    if (!ui_texture.is_valid())
    {
        if (!ui_texture.load_from_file(textures_path("pickup_enemy.png")))
        {
            throw std::runtime_error("Failed to load health texture");
        }
    }

    // Loading shaders
    if (!effect->load_from_file(shader_path("textured.vs.glsl"), shader_path("textured.fs.glsl")))
        throw std::runtime_error("Failed to load texture shaders");

    if (!sprite->initTexture(&ui_texture))
        throw std::runtime_error("Failed to initialize health sprite");

    physics->scale = { -0.08f, 0.08f };
    motion->position = position;

    return true;
}

void WeaponUI::update(float ms) {
}

void WeaponUI::draw(const mat3 &projection) {
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

void WeaponUI::destroy() {
    auto* effect = getComponent<EffectComponent>();
    auto* sprite = getComponent<SpriteComponent>();

    effect->release();
    sprite->release();
    ECS::Entity::destroy();
}

void WeaponUI::setUI(const char* weapon_png) {
    if (!ui_texture.load_from_file(weapon_png)) {
        throw std::runtime_error("Failed to load health texture");
    }
}
