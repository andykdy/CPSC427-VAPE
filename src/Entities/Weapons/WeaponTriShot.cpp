//
// Created by Tanha Kabir on 2019-12-02.
//

#include "WeaponTriShot.hpp"
#include <Entities/Projectiles and Damaging/bullet.hpp>
#include <Engine/GameEngine.hpp>

namespace {
    const size_t BULLET_COOLDOWN_MS = 300;
}

void WeaponTriShot::init() {
    m_bullet_cooldown = -1.f;

    // Load sound
    m_bullet_sound = Mix_LoadWAV(audio_path("pow.wav"));
    if ( m_bullet_sound == nullptr)
    {
        fprintf(stderr, "Failed to load sound pow.wav\n %s\n make sure the data directory is present",
                audio_path("pow.wav"));
        throw std::runtime_error("Failed to load sound pow.wav");
    }
}

void WeaponTriShot::fire(const vec2 &origin_position, float origin_rotation) {
    auto & projectiles = GameEngine::getInstance().getSystemManager()->getSystem<ProjectileSystem>();

    if (m_bullet_cooldown < 0.f) {
        Bullet* bullet1 = &GameEngine::getInstance().getEntityManager()->addEntity<Bullet>();
        if (bullet1->init(origin_position, origin_rotation, false, 5)) {
            m_bullet_cooldown = BULLET_COOLDOWN_MS;
            Mix_PlayChannel(-1, m_bullet_sound, 0);
            projectiles.friendly_projectiles.emplace_back(bullet1);
        } else {
            throw std::runtime_error("Failed to spawn bullet");
        }

        Bullet* bullet2 = &GameEngine::getInstance().getEntityManager()->addEntity<Bullet>();
        if (bullet2->init(origin_position, origin_rotation, false, 5)) {
            m_bullet_cooldown = BULLET_COOLDOWN_MS;
            Mix_PlayChannel(-1, m_bullet_sound, 0);
            projectiles.friendly_projectiles.emplace_back(bullet2);
        } else {
            throw std::runtime_error("Failed to spawn bullet");
        }

        Bullet* bullet3 = &GameEngine::getInstance().getEntityManager()->addEntity<Bullet>();
        if (bullet3->init(origin_position, origin_rotation, false, 5)) {
            m_bullet_cooldown = BULLET_COOLDOWN_MS;
            Mix_PlayChannel(-1, m_bullet_sound, 0);
            projectiles.friendly_projectiles.emplace_back(bullet3);
        } else {
            throw std::runtime_error("Failed to spawn bullet");
        }
    }
}

void WeaponTriShot::update(float ms) {
    m_bullet_cooldown -= ms;
}

void WeaponTriShot::destroy() {
    if (m_bullet_sound != nullptr)
        Mix_FreeChunk(m_bullet_sound);
}
