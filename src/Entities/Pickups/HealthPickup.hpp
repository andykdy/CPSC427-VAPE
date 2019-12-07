//
// Created by Andrea Park on 2019-12-05.
//

#ifndef VAPE_HEALTHPICKUP_HPP
#define VAPE_HEALTHPICKUP_HPP

#include "Pickup.hpp"

class HealthPickup : public Pickup {
    static Texture pickup_texture;
public:
    bool init(vec2 position) override;
    void update(float ms) override;
    void draw(const mat3 &projection) override;
    void destroy() override;

    vec2 get_position() const override;
    bool collides_with(const Player &player) override;
    void applyEffect(Player& player) override;
    vec2 get_bounding_box() const override;
    bool isOffScreen(const vec2 &screen) override;
    char* get_png() override;
private:
};


#endif //VAPE_HEALTHPICKUP_HPP
