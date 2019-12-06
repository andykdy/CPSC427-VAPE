//
// Created by Andy Kim on 12/05/2019.
//

#ifndef VAPE_BOSS3CLONE_HPP
#define VAPE_BOSS3CLONE_HPP

#include <Entities/Bosses/Clone.hpp>
#include "common.hpp"

enum CloneState { moving, stunned, attack };


// Player enemy
class Boss3Clone : public Clone{
    // Shared between all turtles, no need to load one for each instance
    static Texture texture;
public:
// Creates all the associated render resources and default transform
    bool init(vec2 pos, vec2 disp) override;

    // Releases all the associated resources
    void destroy() override;

    // Update turtle due to current
    // ms represents the number of milliseconds elapsed from the previous update() call
    void update(float ms) override;

    // Renders the salmon
    // projection is the 2D orthographic projection matrix
    void draw(const mat3& projection) override;

    // Returns the current turtle position
    vec2 get_position()const override;

    // Sets the new turtle position
    void set_position(vec2 position) override;

    // Returns the turtle' bounding box for collision detection, called by collides_with()
    vec2 get_bounding_box() const override;
	bool collidesWith(Player& player) override; 
	bool checkCollision(vec2 pos, vec2 box) const override;

    void set_velocity(vec2 velocity) override;

	void set_lead();
	vec2 move_to(float ms, vec2 target, float speed);
	void stun() override;
private:
    float m_bullet_cooldown;
    unsigned int m_burst_count;
    float m_burst_cooldown;
	float m_stun_duration;
	vec2 m_master_pos;
	vec2 target_pos;
	CloneState m_curr_state;
	CloneState m_prev_state;
	float m_speed;
	bool m_is_lead;


    void spawnBullet();
};

#endif //VAPE_BOSS3CLONE_HPP
