//
// Created by Cody on 11/10/2019.
//

#ifndef VAPE_VAMPBAR_HPP
#define VAPE_VAMPBAR_HPP

// Contains the bar surrounding/behind the vamp charge, managed by VampCharge

#include <Engine/ECS/Entity.hpp>

class VampBar : public ECS::Entity {
    static Texture vamp_bar_texture; // TODO texture or just box/outline

public:
    bool init(vec2 pos);
    void update(float ms) override {};
    void draw(const mat3& projection) override;
    void destroy() override;
};


#endif //VAPE_VAMPBAR_HPP
