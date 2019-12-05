//
// Created by Andy on 12/03/2019.
//

#ifndef VAPE_MACHINEGUNPICKUP_HPP
#define VAPE_MACHINEGUNPICKUP_HPP


#include "Pickup.hpp"

class MachineGunPickup : public Pickup {
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
private:
};


#endif //VAPE_MACHINEGUNPICKUP_HPP
