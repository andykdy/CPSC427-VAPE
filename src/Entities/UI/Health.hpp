//
// Created by Tanha Kabir on 2019-10-03.
//

#ifndef VAPE_HEALTH_HPP
#define VAPE_HEALTH_HPP

#include "common.hpp"


class Health: public Entity {
    static Texture health_point_texture;

public:
    bool init(vec2 position);
    void update(float ms);
    void draw(const mat3& projection)override;
    void destroy();

    void setHealth(int health);

private:
    int health;
};


#endif //VAPE_HEALTH_HPP
