//
// Created by Cody on 11/14/2019.
//

#include <Components/SpriteComponent.hpp>
#include <Components/EffectComponent.hpp>
#include <Components/PhysicsComponent.hpp>
#include <Components/MotionComponent.hpp>
#include <Components/TransformComponent.hpp>
#include "Cursor.hpp"

Texture Cursor::cursor_texture;

bool Cursor::init(const vec2 &position, const vec2 &scale, float rotation) {
    auto* sprite = addComponent<SpriteComponent>();
    auto* effect = addComponent<EffectComponent>();
    auto* physics = addComponent<PhysicsComponent>();
    auto* motion = addComponent<MotionComponent>();
    auto* transform = addComponent<TransformComponent>();

    // Load shared texture
    if (!cursor_texture.is_valid())
    {
        if (!cursor_texture.load_from_file(textures_path("UI/cursor.png")))
        {
            throw std::runtime_error("Failed to load cursor texture");
        }
    }

    // Loading shaders
    if (!effect->load_from_file(shader_path("textured.vs.glsl"), shader_path("textured.fs.glsl")))
        throw std::runtime_error("Failed to load texture shaders");

    if (!sprite->initTexture(&cursor_texture))
        throw std::runtime_error("Failed to initialize cursor sprite");

    physics->scale = scale;
    motion->position = position;
    motion->radians = rotation;

    return true;
}

void Cursor::destroy() {
    auto* effect = getComponent<EffectComponent>();
    auto* sprite = getComponent<SpriteComponent>();

    effect->release();
    sprite->release();

    ECS::Entity::destroy();
}

void Cursor::draw(const mat3 &projection) {
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

void Cursor::setPosition(const vec2 &position) {
    auto* motion = getComponent<MotionComponent>();
    motion->position = position;
}
