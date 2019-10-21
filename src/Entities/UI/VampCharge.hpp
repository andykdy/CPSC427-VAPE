//
// Created by matte on 10/15/2019.
//

#ifndef VAPE_VAMPCHARGE_HPP
#define VAPE_VAMPCHARGE_HPP


#include "common.hpp"

class VampCharge: public EntityOld {

    static Texture vamp_charge_texture;

    public:
        bool init(vec2 position);
        void update(float ms);
        void draw(const mat3& projection)override;
        void destroy();

        void setVampCharge(int charge);

    private:
        int charge;
    };

#endif //VAPE_VAMPCHARGE_HPP
