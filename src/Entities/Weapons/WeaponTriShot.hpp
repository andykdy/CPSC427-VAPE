//
// Created by Tanha Kabir on 2019-12-02.
//

#ifndef VAPE_WEAPONTRISHOT_HPP
#define VAPE_WEAPONTRISHOT_HPP

#include <SDL_mixer.h>
#include <Utils/PhysFSHelpers.hpp>
#include "Weapon.hpp"

class WeaponTriShot: public Weapon {
private:
    RWFile m_bullet_sound_file;
    Mix_Chunk* m_bullet_sound;
    float m_bullet_cooldown;
public:
    void init() override;

    void fire(const vec2 &origin_position, float origin_rotation) override;

    void update(float ms) override;

    void destroy() override;
};


#endif //VAPE_WEAPONTRISHOT_HPP
