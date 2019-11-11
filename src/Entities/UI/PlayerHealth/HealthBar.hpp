//
// Created by Cody on 11/10/2019.
//

#ifndef VAPE_HEALTHBAR_HPP
#define VAPE_HEALTHBAR_HPP

// Contain the bar surrounding/behind the health pips, managed by Health

#include <Engine/ECS/Entity.hpp>

class HealthBar : public ECS::Entity {
    static Texture health_bar_texture; // TODO texture or just box/outline

public:
    bool init(vec2 pos);
    void update(float ms) override {};
    void draw(const mat3& projection) override;
    void destroy() override;
};


#endif //VAPE_HEALTHBAR_HPP
