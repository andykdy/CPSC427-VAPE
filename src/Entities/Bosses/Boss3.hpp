//
// Created by Andy Kim on 12/05/2019.
//

#ifndef VAPE_BOSS3_HPP
#define VAPE_BOSS3_HPP

#include <Engine/GameEngine.hpp>
#include <Entities/Projectiles and Damaging/bullet.hpp>
#include <Entities/Bosses/Boss3Clone.hpp>
#include <vector>
#include <common.hpp>
#include "Boss.hpp"
enum Boss3State { aiming, charging, cooldown };

class Boss3 : public Boss {
    static Texture boss3_texture;
public:
    // Creates all the associated render resources and default transform
    bool init(vec2 screen) override;

    // Releases all the associated resources
    void destroy() override;

    // Update Boss due to current
    // ms represents the number of milliseconds elapsed from the previous update() call
    void update(float ms) override;

    // Renders the Boss
    // projection is the 2D orthographic projection matrix
    void draw(const mat3& projection) override;

    // Returns the current turtle position
    vec2 get_position()const override;

    // Sets the new Boss position
    void set_position(vec2 position) override;

    // Returns the Boss' bounding box for collision detection, called by collides_with()
    vec2 get_bounding_box() const override;

    void addDamage(int damage) override;

    bool collidesWith(const Vamp& vamp) override;

    bool collidesWith(Player &player) override;

    bool checkCollision(vec2 pos, vec2 box) const override;

private:
	float m_damage_effect_cooldown;
	bool m_is_cloned;
	vec2 m_screen;
	float m_charge_timer;
	float m_cooldown_timer;
	vec2 m_target;
	Boss3State m_curr_state;

	void state1Update(float ms);
	void state2Update(float ms);
	void spawnClones();
	bool nearBounds();
};


#endif //VAPE_BOSS3_HPP
