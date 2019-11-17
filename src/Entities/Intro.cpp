//
// Created by Cody on 11/10/2019.
//

#include <Components/SpriteComponent.hpp>
#include <Components/EffectComponent.hpp>
#include <Components/MotionComponent.hpp>
#include <Components/TransformComponent.hpp>
#include <Engine/GameEngine.hpp>
#include <Levels/Level1.hpp>
#include <Engine/States/LevelState.hpp>
#include "Intro.hpp"


// Same as static in c, local to compilation unit
namespace
{
    const size_t DELAY = 10000;
}

bool Intro::init(vec2 screen) {
    auto* sprite = addComponent<SpriteComponent>();
    auto* effect = addComponent<EffectComponent>();
    auto* motion = addComponent<MotionComponent>();
    auto* transform = addComponent<TransformComponent>();

    // TODO textures
    // Load textures
    std::vector<std::string> paths = {
        textures_path("intro/intro1.png"),
        textures_path("intro/intro2.png"),
    };
    for (auto& path : paths) {
        introTextures.emplace_back(new Texture());
        if (!introTextures.back()->load_from_file(path.c_str())) {
            fprintf(stderr, "Failed to load the intro texture!");
            return false;;
        }
    }

    // Loading shaders
    if (!effect->load_from_file(shader_path("textured.vs.glsl"), shader_path("textured.fs.glsl")))
        return false;

    if (!sprite->initTexture(introTextures.front()))
        throw std::runtime_error("Failed to initialize player sprite");

    // Setting initial values
    motion->position = { screen.x / 2, screen.y /2 };
    m_timer = DELAY;

    return true;
}

void Intro::update(float ms) { m_timer -= ms; }

void Intro::draw(const mat3 &projection) {
    auto* transform = getComponent<TransformComponent>();
    auto* effect = getComponent<EffectComponent>();
    auto* motion = getComponent<MotionComponent>();
    auto* sprite = getComponent<SpriteComponent>();

    transform->begin();
    transform->translate(motion->position);
    transform->scale({1,1});
    transform->rotate(0);
    transform->end();

    sprite->draw(projection, transform->out, effect->program);

    // if (sprite->hasLooped()) {
    if (m_timer <= 0) {
        ++m_part;
        if (m_part < introTextures.size()) {
            sprite->initTexture(introTextures[m_part]);
            m_timer = DELAY;
        } else {
            GameEngine::getInstance().changeState(new LevelState(Levels::level1, 0));
        }
    }
}

void Intro::destroy() {
    auto* effect = getComponent<EffectComponent>();
    auto* sprite = getComponent<SpriteComponent>();

    effect->release();
    sprite->release();

    for (auto tex : introTextures)
        delete tex;
    introTextures.clear();

    ECS::Entity::destroy();
}
