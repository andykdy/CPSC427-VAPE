//
// Created by Cody on 11/10/2019.
//

#ifndef VAPE_HEALTHICON_HPP
#define VAPE_HEALTHICON_HPP

// Contain the health icon, managed by Health

#include <Engine/ECS/ECS.hpp>

class HealthIcon : public ECS::Entity {
    static Texture health_icon_texture;

public:
    bool init(vec2 pos);
    void update(float ms) override {};
    void draw(const mat3& projection) override;
    void destroy() override;
};


#endif //VAPE_HEALTHICON_HPP
