//
// Created by Cody on 10/17/2019.
//

#ifndef VAPE_HEALTHCOMPONENT_HPP
#define VAPE_HEALTHCOMPONENT_HPP

#include <Engine/ECS/Component.hpp>

class HealthComponent : public ECS::Component {
public:
    float m_health = 0;
    float lose_health(float amount) {
        m_health -= amount;
        return m_health;
    }

    float gain_health(float amount) {
        m_health += amount;
        return m_health;
    }

    float get_health() const {
        return m_health;
    }

    bool is_alive() {
        return m_health > 0;
    }

    void set_health(float init_health) {
        m_health = init_health;
    }
};
#endif //VAPE_HEALTHCOMPONENT_HPP
