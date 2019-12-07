//
// Created by matte on 2019-12-05.
//

#include "Score.hpp"
#include <cmath>
#include <algorithm>
#include <Components/EffectComponent.hpp>
#include <Components/PhysicsComponent.hpp>
#include <Components/MotionComponent.hpp>
#include <Components/TransformComponent.hpp>
#include <Components/SpriteComponent.hpp>
#include <Engine/GameEngine.hpp>

bool Score::init(vec2 position, Font* font) {
    auto* sprite = addComponent<SpriteComponent>();
    auto* effect = addComponent<EffectComponent>();
    auto* physics = addComponent<PhysicsComponent>();
    auto* motion = addComponent<MotionComponent>();
    auto* transform = addComponent<TransformComponent>();

    // Loading shaders
    if (!effect->load_from_file(shader_path("textured.vs.glsl"), shader_path("textured.fs.glsl")))
        throw std::runtime_error("Failed to load texture shaders");

    score_font = font;
    score_text.init(score_font);

//    physics->scale = { 0.7f, 1.f };
    motion->position = { position.x, position.y };

    return true;
}

void Score::update(float ms) {
}

void Score::draw(const mat3 &projection) {
    auto* transform = getComponent<TransformComponent>();
    auto* effect = getComponent<EffectComponent>();
    auto* motion = getComponent<MotionComponent>();
    auto* physics = getComponent<PhysicsComponent>();
    auto* sprite = getComponent<SpriteComponent>();

    // Transformation code, see Rendering and Transformation in the template specification for more info
    // Incrementally updates transformation matrix, thus ORDER IS IMPORTANT

    std::string score_str = std::to_string(score);
    std::string padded_score = std::string(7 - score_str.length(), '0') + score_str;
    // add zeros to score to fill it out
    char const *pchar = padded_score.c_str();
    score_text.setText(pchar);
    score_text.setColor({1.f, 0.8f, 0.0f});
    score_text.setPosition(motion->position);
    score_text.setScale( {0.85f, 0.85f});

    score_text.draw(projection);
}

void Score::destroy() {
    score_text.destroy();
    auto* effect = getComponent<EffectComponent>();
    auto* sprite = getComponent<SpriteComponent>();

    effect->release();
    sprite->release();
    ECS::Entity::destroy();
}

void Score::setScore(int score) {
    Score::score = score;
}
