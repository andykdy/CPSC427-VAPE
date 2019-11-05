//
// Created by matte on 10/15/2019.
//

#include <iostream>
#include <Components/EffectComponent.hpp>
#include <Components/SpriteComponent.hpp>
#include <Components/PhysicsComponent.hpp>
#include <Components/MotionComponent.hpp>
#include <Components/TransformComponent.hpp>
#include "VampCharge.hpp"


Texture VampCharge::vamp_charge_texture;

bool VampCharge::init(vec2 position) {
    auto* sprite = addComponent<SpriteComponent>();
    auto* effect = addComponent<EffectComponent>();
    auto* physics = addComponent<PhysicsComponent>();
    auto* motion = addComponent<MotionComponent>();
    auto* transform = addComponent<TransformComponent>();

    // Load shared texture
    if (!vamp_charge_texture.is_valid())
    {
        if (!vamp_charge_texture.load_from_file(textures_path("vamp_charge.png")))
        {
            fprintf(stderr, "Failed to load vamp_charge texture!");
            return false;
        }
    }

    if (gl_has_errors())
        return false;

    // Loading shaders
    if (!effect->load_from_file(shader_path("textured.vs.glsl"), shader_path("textured.fs.glsl")))
        return false;

    if (!sprite->initTexture(&vamp_charge_texture))
        throw std::runtime_error("Failed to initialize health sprite");

    physics->scale = { 0.25f, 0.25f };
    motion->position = { position.x, position.y };
    charge = 0;

    ECS::Entity::setActive();

    return !gl_has_errors();
}

void VampCharge::update(float ms) {
}

void VampCharge::draw(const mat3 &projection) {
    // Transformation code, see Rendering and Transformation in the template specification for more info
    // Incrementally updates transformation matrix, thus ORDER IS IMPORTANT

    auto* transform = getComponent<TransformComponent>();
    auto* effect = getComponent<EffectComponent>();
    auto* motion = getComponent<MotionComponent>();
    auto* physics = getComponent<PhysicsComponent>();
    auto* sprite = getComponent<SpriteComponent>();

    int num = 4*charge;
    for (int i = 0; i < num; i++) {
        transform->begin();
        vec2 offset = {(float)i/2.f * 5.f, 0.f};
        if (i%2 == 0) {
            offset = {(float)i/2.f * - 5.f, 0.f};
        }
        offset.x += motion->position.x;
        offset.y += motion->position.y;
        transform->translate(offset);
        transform->scale(physics->scale);
        transform->end();

        sprite->draw(projection, transform->out, effect->program);
    }
}

void VampCharge::destroy() {
    auto* effect = getComponent<EffectComponent>();
    auto* sprite = getComponent<SpriteComponent>();

    effect->release();
    sprite->release();
    ECS::Entity::destroy();
}

void VampCharge::setVampCharge(int charge) {
    VampCharge::charge = charge;
}

VampCharge::VampCharge(ECS::EntityId id) : Entity(id) {}
