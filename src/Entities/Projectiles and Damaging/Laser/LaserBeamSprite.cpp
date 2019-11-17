//
// Created by Cody on 11/16/2019.
//

#include <Components/EffectComponent.hpp>
#include <Components/SpriteComponent.hpp>
#include <Components/TransformComponent.hpp>
#include "LaserBeamSprite.hpp"

constexpr size_t SPRITE_FRAMES = 4;
constexpr size_t SPRITE_W = 32;
constexpr size_t SPRITE_H = 1128;

Texture LaserBeamSprite::laser_texture;

bool LaserBeamSprite::init(vec2 origin) {
    gl_flush_errors();
    auto* transform = addComponent<TransformComponent>();
    auto* effect = addComponent<EffectComponent>();
    auto* sprite = addComponent<SpriteComponent>();

    // Load shared texture
    if (!laser_texture.is_valid())
    {
        if (!laser_texture.load_from_file(textures_path("Laser3.png")))
        {
            fprintf(stderr, "Failed to load laser texture!");
            return false;
        }
    }

    if (!effect->load_from_file(shader_path("textured.vs.glsl"), shader_path("textured.fs.glsl")))
        return false;

    if (!sprite->initTexture(&laser_texture, SPRITE_FRAMES, SPRITE_W, SPRITE_H, -0.03f))
        throw std::runtime_error("Failed to initialize laser sprite");

    if (gl_has_errors())
        return false;

    m_origin = origin;

    return true;
}

void LaserBeamSprite::draw(const mat3 &projection, float rotation) {
    auto* transform = getComponent<TransformComponent>();
    auto* effect = getComponent<EffectComponent>();
    auto* sprite = getComponent<SpriteComponent>();

    vec2 offset = { m_origin.x  + SPRITE_H/2*sinf(rotation), m_origin.y + SPRITE_H/2*cosf(rotation)};
    transform->begin();
    transform->translate(offset);
    transform->scale({1,1});
    transform->rotate(-rotation);
    transform->end();

    sprite->draw(projection, transform->out, effect->program);
}

void LaserBeamSprite::destroy() {
    auto* effect = getComponent<EffectComponent>();
    auto* sprite = getComponent<SpriteComponent>();

    effect->release();
    sprite->release();
    ECS::Entity::destroy();
}

