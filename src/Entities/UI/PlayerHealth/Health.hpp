//
// Created by Tanha Kabir on 2019-10-03.
//

#ifndef VAPE_HEALTH_HPP
#define VAPE_HEALTH_HPP

#include "common.hpp"
#include "Engine/ECS/ECS.hpp"
#include "HealthBar.hpp"
#include "HealthIcon.hpp"

class Health: public ECS::Entity {
    static Texture health_point_texture;

public:
    bool init(vec2 position);
    void update(float ms) override;
    void draw(const mat3& projection) override;
    void destroy() override;

    void setHealth(int health);

private:
    HealthBar* m_bar;
    HealthIcon* m_icon;
    int health = 1;
};


#endif //VAPE_HEALTH_HPP
