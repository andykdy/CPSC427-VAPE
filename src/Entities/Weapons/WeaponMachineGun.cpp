//
// Created by Tanha Kabir on 2019-12-03.
//

#include "WeaponMachineGun.hpp"

#include <Entities/Projectiles and Damaging/bullet.hpp>
#include <Engine/GameEngine.hpp>

namespace {
    const size_t BULLET_COOLDOWN_MS = 150;
    const size_t TOTAL_AMO = 100;
}

void WeaponMachineGun::init() {
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

    amo = TOTAL_AMO;
}

void WeaponMachineGun::fire(const vec2 &origin_position, float origin_rotation) {
    auto & projectiles = GameEngine::getInstance().getSystemManager()->getSystem<ProjectileSystem>();

    if (m_bullet_cooldown < 0.f) {
        amo -= 1;

        Bullet* bullet = &GameEngine::getInstance().getEntityManager()->addEntity<Bullet>();
        if (bullet->init(origin_position, origin_rotation, false, 5)) {
            m_bullet_cooldown = BULLET_COOLDOWN_MS;
            Mix_PlayChannel(-1, m_bullet_sound, 0);
            projectiles.friendly_projectiles.emplace_back(bullet);
        } else {
            throw std::runtime_error("Failed to spawn bullet");
        }
    }
}

void WeaponMachineGun::update(float ms) {
    m_bullet_cooldown -= ms;
}

void WeaponMachineGun::destroy() {
    if (m_bullet_sound != nullptr)
        Mix_FreeChunk(m_bullet_sound);
    m_bullet_sound_file.destroy();
}
