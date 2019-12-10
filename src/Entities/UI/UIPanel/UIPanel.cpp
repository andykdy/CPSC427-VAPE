//
// Created by matte on 2019-11-12.
//

#include <Components/TransformComponent.hpp>
#include <Components/EffectComponent.hpp>
#include <Components/MeshComponent.hpp>
#include <Components/SpriteComponent.hpp>
#include <Components/MotionComponent.hpp>
#include <Components/PhysicsComponent.hpp>
#include <Engine/GameEngine.hpp>
#include "UIPanelBackground.hpp"
#include "UIPanel.hpp"

Texture UIPanel::UI_texture;

bool UIPanel::init(vec2 screen, float height, float width) {

    auto* sprite = addComponent<SpriteComponent>();
    auto* effect = addComponent<EffectComponent>();
    auto* physics = addComponent<PhysicsComponent>();
    auto* motion = addComponent<MotionComponent>();
    auto* transform = addComponent<TransformComponent>();

    // Load texture
    if (!UI_texture.is_valid())
    {
        if (!UI_texture.load_from_file(textures_path("UI.png")))
        {
            fprintf(stderr, "Failed to load UI_texture texture!");
            return false;
        }
    }

    if (gl_has_errors())
        return false;

    // Loading shaders
    if (!effect->load_from_file(shader_path("textured.vs.glsl"), shader_path("textured.fs.glsl")))
        return false;

    if (!sprite->initTexture(&UI_texture))
        throw std::runtime_error("Failed to initialize health sprite");

    physics->scale = { 1.f, 1.f};
    motion->position = { width * .5f, height * 0.95f};
//    motion->position = { height * 0.55f, width * .5f };

    return !gl_has_errors();

}

void UIPanel::draw(const mat3 &projection) {

    //std::cout << "Drawing here " << std::endl;
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

void UIPanel::destroy() {
    auto* effect = getComponent<EffectComponent>();
    auto* sprite = getComponent<SpriteComponent>();

    effect->release();
    sprite->release();
    ECS::Entity::destroy();
    Entity::destroy();
}
