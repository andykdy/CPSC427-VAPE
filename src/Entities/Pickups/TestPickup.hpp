//
// Created by Cody on 11/10/2019.
//

#ifndef VAPE_TESTPICKUP_HPP
#define VAPE_TESTPICKUP_HPP


#include "Pickup.hpp"

class TestPickup : public Pickup {
	static Texture pickup_texture;
public:
    bool init(vec2 position) override;
    void update(float ms) override;
    void draw(const mat3 &projection) override;
    void destroy() override;

    vec2 get_position() const override;
    bool collides_with(const Player &player) override;
    void applyEffect(LevelState& level) override;
    bool isOffScreen(const vec2 &screen) override;
};


#endif //VAPE_TESTPICKUP_HPP
