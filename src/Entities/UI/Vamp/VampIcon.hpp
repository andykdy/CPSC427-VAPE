//
// Created by Cody on 11/10/2019.
//

#ifndef VAPE_VAMPICON_HPP
#define VAPE_VAMPICON_HPP

// Contains the vamp charge icon, managed by VampCharge

#include <Engine/ECS/Entity.hpp>

class VampIcon : public ECS::Entity {
    static Texture vamp_icon_texture;

public:
    bool init(vec2 pos);
    void update(float ms) override {};
    void draw(const mat3& projection) override;
    void destroy() override;
};


#endif //VAPE_VAMPICON_HPP
