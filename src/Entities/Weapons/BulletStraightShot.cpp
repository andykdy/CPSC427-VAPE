//
// Created by Cody on 11/20/2019.
//

#include <Entities/Projectiles and Damaging/bullet.hpp>
#include <Engine/GameEngine.hpp>
#include "BulletStraightShot.hpp"

namespace {
    const size_t BULLET_COOLDOWN_MS = 300;
}

void BulletStraightShot::init() {
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

void BulletStraightShot::fire(const vec2 &origin_position, float origin_rotation) {
    if (m_bullet_cooldown < 0.f) {
        Bullet* bullet = &GameEngine::getInstance().getEntityManager()->addEntity<Bullet>();
        if (bullet->init(origin_position, origin_rotation)) {
            m_projectiles.emplace_back(bullet);
        } else {
            throw std::runtime_error("Failed to spawn bullet");
        }
        m_bullet_cooldown = BULLET_COOLDOWN_MS;
        Mix_PlayChannel(-1, m_bullet_sound, 0);
    }
}

void BulletStraightShot::update(float ms) {
    m_bullet_cooldown -= ms;

    // Get screen size
    int w, h;
    glfwGetFramebufferSize(GameEngine::getInstance().getM_window(), &w, &h);
    vec2 screen = { (float)w / GameEngine::getInstance().getM_screen_scale(), (float)h / GameEngine::getInstance().getM_screen_scale() };

    // Update bullets, removing out of screen bullets
    auto bullet_it = m_projectiles.begin();
    while(bullet_it != m_projectiles.end()) {
        if ((*bullet_it)->isOffScreen(screen))
        {
            (*bullet_it)->destroy();
            bullet_it = m_projectiles.erase(bullet_it);
            continue;
        } else {
            (*bullet_it)->update(ms);
        }

        ++bullet_it;
    }
}

void BulletStraightShot::destroy() {
    if (m_bullet_sound != nullptr)
        Mix_FreeChunk(m_bullet_sound);
    Weapon::destroy();
}

void BulletStraightShot::draw(const mat3 &projection) {
    for (auto bullet : m_projectiles)
        bullet->draw(projection);
}
