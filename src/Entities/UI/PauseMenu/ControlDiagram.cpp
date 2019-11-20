//
// Created by Cody on 11/19/2019.
//

#include <Components/SpriteComponent.hpp>
#include <Components/EffectComponent.hpp>
#include <Components/PhysicsComponent.hpp>
#include <Components/MotionComponent.hpp>
#include <Components/TransformComponent.hpp>
#include "ControlDiagram.hpp"

Texture ControlDiagram::controls_texture;

bool ControlDiagram::init(const vec2 position, const vec2 scale, float rotation) {
    auto* sprite = addComponent<SpriteComponent>();
    auto* effect = addComponent<EffectComponent>();
    auto* physics = addComponent<PhysicsComponent>();
    auto* motion = addComponent<MotionComponent>();
    auto* transform = addComponent<TransformComponent>();

    // Load shared texture
    if (!controls_texture.is_valid())
    {
        if (!controls_texture.load_from_file(textures_path("controls.png")))
        {
            throw std::runtime_error("Failed to load controls texture");
        }
    }

    // Loading shaders
    if (!effect->load_from_file(shader_path("textured.vs.glsl"), shader_path("textured.fs.glsl")))
        throw std::runtime_error("Failed to load texture shaders");

    if (!sprite->initTexture(&controls_texture))
        throw std::runtime_error("Failed to initialize controls sprite");

    motion->position = position;
    physics->scale = scale;
    motion->radians = rotation;
    return true;
}

void ControlDiagram::destroy() {
    auto* effect = getComponent<EffectComponent>();
    auto* sprite = getComponent<SpriteComponent>();

    effect->release();
    sprite->release();

    ECS::Entity::destroy();
}

void ControlDiagram::update(float ms) {

}

void ControlDiagram::draw(const mat3 &projection) {
    auto* transform = getComponent<TransformComponent>();
    auto* effect = getComponent<EffectComponent>();
    auto* sprite = getComponent<SpriteComponent>();
    auto* motion = getComponent<MotionComponent>();
    auto* physics = getComponent<PhysicsComponent>();

    transform->begin();
    transform->translate(motion->position);
    transform->scale(physics->scale);
    transform->rotate(motion->radians);
    transform->end();

    sprite->draw(projection, transform->out, effect->program);
}
