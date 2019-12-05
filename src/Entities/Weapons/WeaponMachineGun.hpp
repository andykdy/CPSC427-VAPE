//
// Created by Tanha Kabir on 2019-12-03.
//

#ifndef VAPE_WEAPONMACHINEGUN_HPP
#define VAPE_WEAPONMACHINEGUN_HPP

#include <SDL_mixer.h>
#include "Weapon.hpp"

class WeaponMachineGun: public Weapon {
private:
    Mix_Chunk* m_bullet_sound;
    float m_bullet_cooldown;
public:
    void init() override;

    void fire(const vec2 &origin_position, float origin_rotation) override;

    void update(float ms) override;

    void destroy() override;
};

#endif //VAPE_WEAPONMACHINEGUN_HPP
