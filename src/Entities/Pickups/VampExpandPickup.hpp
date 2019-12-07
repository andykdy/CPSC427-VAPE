//
// Created by Andrea Park on 2019-12-05.
//

#ifndef VAPE_VAMPEXPANDPICKUP_HPP
#define VAPE_VAMPEXPANDPICKUP_HPP

#include "Pickup.hpp"

class VampExpandPickup : public Pickup {
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
    const char* get_png() const override;
    bool isWeapon() override;
private:
};

#endif //VAPE_VAMPEXPANDPICKUP_HPP
