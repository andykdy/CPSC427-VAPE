//
// Created by Cody on 11/14/2019.
//

#include <Components/SpriteComponent.hpp>
#include <Components/EffectComponent.hpp>
#include <Components/PhysicsComponent.hpp>
#include <Components/MotionComponent.hpp>
#include <Components/TransformComponent.hpp>
#include <Engine/GameEngine.hpp>
#include "ExitButton.hpp"

Texture ExitButton::exit_button_texture;

bool ExitButton::init(const vec2 &position, const vec2 &scale, const float rotation) {
    auto* sprite = addComponent<SpriteComponent>();
    auto* effect = addComponent<EffectComponent>();
    auto* physics = addComponent<PhysicsComponent>();
    auto* motion = addComponent<MotionComponent>();
    auto* transform = addComponent<TransformComponent>();

    // Load shared texture
    if (!exit_button_texture.is_valid())
    {
        if (!exit_button_texture.load_from_file(textures_path("UI/button_exit.png")))
        {
            throw std::runtime_error("Failed to load exit button texture");
        }
    }

    // Loading shaders
    if (!effect->load_from_file(shader_path("textured.vs.glsl"), shader_path("textured.fs.glsl")))
        throw std::runtime_error("Failed to load texture shaders");

    if (!sprite->initTexture(&exit_button_texture))
        throw std::runtime_error("Failed to initialize exit button sprite");

    physics->scale = scale;
    motion->position = position;
    motion->radians = rotation;
    return true;
}

void ExitButton::destroy() {
    auto* effect = getComponent<EffectComponent>();
    auto* sprite = getComponent<SpriteComponent>();

    effect->release();
    sprite->release();

    ECS::Entity::destroy();
}

void ExitButton::update(const float ms, const vec2 &mouse_position) {
    // TODO
}

void ExitButton::draw(const mat3 &projection) {
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

bool ExitButton::isWithin(const vec2 &mouse_position) {
    auto* physics = getComponent<PhysicsComponent>();
    auto* motion = getComponent<MotionComponent>();
    float w = std::fabs(physics->scale.x) * exit_button_texture.width;
    float h = std::fabs(physics->scale.y) * exit_button_texture.height;
    return ( (mouse_position.x >= motion->position.x-w && mouse_position.x <= motion->position.x+w)
             && (mouse_position.y >= motion->position.y-h && mouse_position.y <= motion->position.y+h));
}

void ExitButton::doAction() {
    GameEngine::getInstance().quit();
}
