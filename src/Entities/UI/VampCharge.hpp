//
// Created by matte on 10/15/2019.
//

#ifndef VAPE_VAMPCHARGE_HPP
#define VAPE_VAMPCHARGE_HPP


#include <Engine/ECS/Entity.hpp>
#include "common.hpp"

class VampCharge: public ECS::Entity {
    static Texture vamp_charge_texture;
public:
    VampCharge(ECS::EntityId id);

    bool init(vec2 position);
    void update(float ms) override;
    void draw(const mat3& projection)override;
    void destroy() override;

    void setVampCharge(int charge);

private:
    int charge = 0;
};

#endif //VAPE_VAMPCHARGE_HPP
