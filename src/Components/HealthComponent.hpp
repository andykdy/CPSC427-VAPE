//
// Created by Cody on 10/17/2019.
//

#ifndef VAPE_HEALTHCOMPONENT_HPP
#define VAPE_HEALTHCOMPONENT_HPP

#include <Engine/ECS/Component.hpp>

class HealthComponent : public ECS::Component {
private:
    int m_health;
public:
    // Kills the salmon, changing its alive state and triggering on death events
    int lose_health(float amount) {
        m_health -= amount;
        return m_health;
    }

    // Gain health after draining an enemy in vamp mode
    int gain_health(float amount) {
        m_health += amount;
        return m_health;
    }

    int get_health() const {
        return m_health;
    }

    bool is_alive() {
        return m_health > 0;
    }
};
#endif //VAPE_HEALTHCOMPONENT_HPP
