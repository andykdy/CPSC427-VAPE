//
// Created by matte on 2019-12-06.
//

#include "WeaponUI.hpp"
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
    const float INF_LOC_X = 368;
    const float INF_LOC_Y = 970;
    const float AMMO_LOC_X = 355;
    const float AMMO_LOC_Y = 970;
}

bool WeaponUI::init(Font* font_ranger, Font* font_cond) {
    auto* sprite = addComponent<SpriteComponent>();
    auto* effect = addComponent<EffectComponent>();
    auto* physics = addComponent<PhysicsComponent>();
    auto* motion = addComponent<MotionComponent>();
    auto* transform = addComponent<TransformComponent>();

    // Loading shaders
    if (!effect->load_from_file(shader_path("textured.vs.glsl"), shader_path("textured.fs.glsl")))
        throw std::runtime_error("Failed to load texture shaders");

    ammo_font = font_ranger;
    inf_font = font_cond;
    inf_text.init(inf_font);
    ammo_text.init(ammo_font);
    alt_wep = false;


    inf_text.init(ammo_font);
    std::string inf_str = ("INF");

    char const *pchar = inf_str.c_str();
    inf_text.setText(pchar);
    inf_text.setColor({1.f, 0.8f, 0.0f});
    inf_text.setPosition({INF_LOC_X, INF_LOC_Y});
    inf_text.setScale( {1.25f, 1.25f});


    ammo_text.setColor({1.f, 0.8f, 0.0f});
    ammo_text.setPosition({AMMO_LOC_X, AMMO_LOC_Y});
    ammo_text.setScale( {1.25f, 1.25f});

    return true;
}

void WeaponUI::update(float ms) {
}

void WeaponUI::setAmmo(float ammo) {
    if ((int) ammo > 0) {

        std::string ammo_str = std::to_string((int) ammo);
        std::string padded_ammo = std::string(3 - ammo_str.length(), '0') + ammo_str;
        char const *pchar = padded_ammo.c_str();
        ammo_text.setText(pchar);
        alt_wep = true;

    } else {
        alt_wep = false;
    }
}


void WeaponUI::draw(const mat3 &projection) {
    // Transformation code, see Rendering and Transformation in the template specification for more info
    // Incrementally updates transformation matrix, thus ORDER IS IMPORTANT

    if (alt_wep) {
        ammo_text.draw(projection);
    } else {
        inf_text.draw(projection);
    }
}

void WeaponUI::destroy() {
    ammo_text.destroy();
    inf_text.destroy();

    auto* effect = getComponent<EffectComponent>();
    auto* sprite = getComponent<SpriteComponent>();

    effect->release();
    sprite->release();
    ECS::Entity::destroy();
}
