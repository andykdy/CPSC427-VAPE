//
// Created by Cody on 11/20/2019.
//

#include "Weapon.hpp"

void Weapon::destroy() {
    for (auto projectile : m_projectiles)
        projectile->destroy();
    m_projectiles.clear();
}
