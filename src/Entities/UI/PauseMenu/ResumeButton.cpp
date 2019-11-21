//
// Created by Cody on 11/19/2019.
//

#include <Components/SpriteComponent.hpp>
#include <Components/EffectComponent.hpp>
#include <Components/PhysicsComponent.hpp>
#include <Components/MotionComponent.hpp>
#include <Components/TransformComponent.hpp>
#include "ResumeButton.hpp"

Texture ResumeButton::resume_button_texture;

bool ResumeButton::init(const vec2 &position, const vec2 &scale, float rotation) {
    auto* sprite = addComponent<SpriteComponent>();
    auto* effect = addComponent<EffectComponent>();
    auto* physics = addComponent<PhysicsComponent>();
    auto* motion = addComponent<MotionComponent>();
    auto* transform = addComponent<TransformComponent>();

    // Load shared texture
    if (!resume_button_texture.is_valid())
    {
        if (!resume_button_texture.load_from_file(textures_path("UI/button_continue.png")))
        {
            throw std::runtime_error("Failed to load resume button texture");
        }
    }

    // Loading shaders
    if (!effect->load_from_file(shader_path("textured.vs.glsl"), shader_path("textured.fs.glsl")))
        throw std::runtime_error("Failed to load texture shaders");

    if (!sprite->initTexture(&resume_button_texture))
        throw std::runtime_error("Failed to initialize resume button sprite");

    physics->scale = scale;
    motion->position = position;
    motion->radians = rotation;

    return true;
}

void ResumeButton::destroy() {
    auto* effect = getComponent<EffectComponent>();
    auto* sprite = getComponent<SpriteComponent>();

    effect->release();
    sprite->release();

    ECS::Entity::destroy();
}

void ResumeButton::update(float ms, const vec2 &mouse_position) {

}

void ResumeButton::draw(const mat3 &projection) {
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

    vec3 color = {1,1,1};
    if (!isSelected())
        color = {0.8f, 0.8f, 0.8f};
    sprite->draw(projection, transform->out, effect->program, color);
}

bool ResumeButton::isWithin(const vec2 &mouse_position) {
    auto* physics = getComponent<PhysicsComponent>();
    auto* motion = getComponent<MotionComponent>();
    float w = std::fabs(physics->scale.x) * resume_button_texture.width;
    float h = std::fabs(physics->scale.y) * resume_button_texture.height;
    return ( (mouse_position.x >= motion->position.x-w/2 && mouse_position.x <= motion->position.x+w/2)
             && (mouse_position.y >= motion->position.y-h/2 && mouse_position.y <= motion->position.y+h/2));
}

vec2 ResumeButton::getPosition() {
    auto* motion = getComponent<MotionComponent>();
    return motion->position;
}
