//
// Created by Cody on 11/20/2019.
//

#include <Entities/Projectiles and Damaging/bullet.hpp>
#include <Engine/GameEngine.hpp>
#include <Utils/PhysFSHelpers.hpp>
#include "BulletStraightShot.hpp"

namespace {
    const size_t BULLET_COOLDOWN_MS = 300;
}

void BulletStraightShot::init() {
    m_bullet_cooldown = -1.f;

    // Load sound
    m_bullet_sound_file.init(audio_path("pow.wav"));
    m_bullet_sound = Load_Wav(m_bullet_sound_file);
    if ( m_bullet_sound == nullptr)
    {
        fprintf(stderr, "Failed to load sound pow.wav\n %s\n make sure the data directory is present",
                audio_path("pow.wav"));
        throw std::runtime_error("Failed to load sound pow.wav");
    }
}

Projectile* BulletStraightShot::fire(const vec2 &origin_position, float origin_rotation) {
    if (m_bullet_cooldown < 0.f) {
        Bullet* bullet = &GameEngine::getInstance().getEntityManager()->addEntity<Bullet>();
        if (bullet->init(origin_position, origin_rotation, false, 5)) {
            m_bullet_cooldown = BULLET_COOLDOWN_MS;
            Mix_PlayChannel(-1, m_bullet_sound, 0);
            return bullet;
        } else {
            throw std::runtime_error("Failed to spawn bullet");
        }
    }
    return nullptr;
}

void BulletStraightShot::update(float ms) {
    m_bullet_cooldown -= ms;
}

void BulletStraightShot::destroy() {
    if (m_bullet_sound != nullptr)
        Mix_FreeChunk(m_bullet_sound);
    m_bullet_sound_file.destroy();
}
