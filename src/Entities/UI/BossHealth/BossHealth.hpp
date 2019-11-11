//
// Created by Cody on 11/9/2019.
//

#ifndef VAPE_BOSSHEALTH_HPP
#define VAPE_BOSSHEALTH_HPP


#include <Engine/ECS/Entity.hpp>
#include "BossHealthBar.hpp"

class BossHealth : public ECS::Entity {
public:
    bool init(vec2 screen, int maxHealth);
    void update(float ms) override {};
    void draw(const mat3& projection) override;
    void destroy() override;

    void setHealth(int health);

private:
    BossHealthBar* m_bar;
    int m_health = 1;
    vec2 m_screen;
};


#endif //VAPE_BOSSHEALTH_HPP
