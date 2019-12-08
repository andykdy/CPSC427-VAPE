//
// Created by matte on 2019-12-06.
//

#include "Lives.hpp"
#include <cmath>
#include <algorithm>
#include <Components/EffectComponent.hpp>
#include <Components/PhysicsComponent.hpp>
#include <Components/MotionComponent.hpp>
#include <Components/TransformComponent.hpp>
#include <Components/SpriteComponent.hpp>
#include <Engine/GameEngine.hpp>

namespace
{
    const std::string LIVES_TEXT = "LIVES: ";
}

bool Lives::init(vec2 position, Font* font, int player_lives) {
    auto* sprite = addComponent<SpriteComponent>();
    auto* effect = addComponent<EffectComponent>();
    auto* physics = addComponent<PhysicsComponent>();
    auto* motion = addComponent<MotionComponent>();
    auto* transform = addComponent<TransformComponent>();

    // Loading shaders
    if (!effect->load_from_file(shader_path("textured.vs.glsl"), shader_path("textured.fs.glsl")))
        throw std::runtime_error("Failed to load texture shaders");

    lives_font = font;
    lives_text.init(lives_font);
    lives = player_lives;
    motion->position = { position.x, position.y };

    return true;
}

void Lives::update(float ms) {
}

void Lives::draw(const mat3 &projection) {
    auto* transform = getComponent<TransformComponent>();
    auto* effect = getComponent<EffectComponent>();
    auto* motion = getComponent<MotionComponent>();
    auto* physics = getComponent<PhysicsComponent>();
    auto* sprite = getComponent<SpriteComponent>();

    // Transformation code, see Rendering and Transformation in the template specification for more info
    // Incrementally updates transformation matrix, thus ORDER IS IMPORTANT

    std::string lives_str = std::to_string(lives);
    std::string padded_score = std::string(2 - lives_str.length(), '0') + lives_str;
    std::string formatted_score = LIVES_TEXT + padded_score;
    // add zeros to score to fill it out
    char const *pchar = formatted_score.c_str();
    lives_text.setText(pchar);
    lives_text.setColor({1.f, 0.8f, 0.0f});
    lives_text.setPosition(motion->position);
    lives_text.setScale( {0.85f, 0.85f});

    lives_text.draw(projection);
}

void Lives::setLives(int player_lives) {
    lives = player_lives;
}

void Lives::destroy() {
    lives_text.destroy();
    auto* effect = getComponent<EffectComponent>();
    auto* sprite = getComponent<SpriteComponent>();

    effect->release();
    sprite->release();
    ECS::Entity::destroy();
}