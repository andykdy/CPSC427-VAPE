//
// Created by Tanha Kabir on 2019-10-03.
//

#ifndef VAPE_HEALTHHEART_HPP
#define VAPE_HEALTHHEART_HPP

#include "common.hpp"


class HealthHeart: public Entity {
    static Texture health_heart_texture;

public:
    bool init(vec2 position);
    void update(float ms);
    void draw(const mat3& projection)override;
    void destroy();
};


#endif //VAPE_HEALTHHEART_HPP
