//
// Created by Cody on 11/10/2019.
//

#include <Components/SpriteComponent.hpp>
#include <Components/EffectComponent.hpp>
#include <Components/MotionComponent.hpp>
#include <Components/TransformComponent.hpp>
#include <Engine/GameEngine.hpp>
#include <Engine/States/TutorialState.hpp>
#include "Intro.hpp"


// Same as static in c, local to compilation unit
namespace
{

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
        textures_path("intro/intro3.png"),
        textures_path("intro/intro4.png"),
        textures_path("intro/intro5.png"),
        textures_path("intro/intro6.png"),
        textures_path("intro/intro7.png"),
    };
    for (auto& path : paths) {
        Texture t;
        if (!t.load_from_file(path.c_str()))
        {
            fprintf(stderr, "Failed to load the intro texture!");
            return false;
        }
        introTextures.push_back(t);
    }

    // Loading shaders
    if (!effect->load_from_file(shader_path("textured.vs.glsl"), shader_path("textured.fs.glsl")))
        return false;

    if (!sprite->initTexture(&introTextures.front(), 100, 800, 1000))
        throw std::runtime_error("Failed to initialize player sprite");

    // Setting initial values
    motion->position = { screen.x / 2, screen.y /2 };

    return true;
}

void Intro::update(float ms) {}

void Intro::draw(const mat3 &projection) {
    auto* transform = getComponent<TransformComponent>();
    auto* effect = getComponent<EffectComponent>();
    auto* motion = getComponent<MotionComponent>();
    auto* sprite = getComponent<SpriteComponent>();

    transform->begin();
    transform->translate(motion->position);
    transform->scale({1,1});
    transform->rotate(motion->radians);
    transform->end();

    sprite->draw(projection, transform->out, effect->program);

    if (sprite->hasLooped()) {
        ++m_part;
        if (m_part < introTextures.size()) {
            sprite->initTexture(&introTextures[m_part], 100,800,1000);
        } else {
            GameEngine::getInstance().changeState(new TutorialState());
        }
    }
}

void Intro::destroy() {
    auto* effect = getComponent<EffectComponent>();
    auto* sprite = getComponent<SpriteComponent>();

    effect->release();
    sprite->release();

    for (auto& tex : introTextures)
        tex.invalidate();
    introTextures.clear();

    ECS::Entity::destroy();
}
