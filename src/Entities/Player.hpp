#pragma once

#include <map>
#include "common.hpp"
#include "Entities/Projectiles and Damaging/Projectile.hpp"
#include <vector>
#include <Engine/ECS/Entity.hpp>
#include <Entities/Weapons/Weapon.hpp>
#include "Entities/Enemies/Enemy.hpp"

class Projectile;
class Weapon;
class Enemy;

class Player : public ECS::Entity
{
	static Texture player_texture;
	static Texture vamp_texture;
public:
// Creates all the associated render resources and default transform
	bool init(vec2 screen, int hp);

	// Releases all associated resources
	void destroy() override;
	
	// Update salmon position based on direction
	// ms represents the number of milliseconds elapsed from the previous update() call
	void update(float ms, std::map<int, bool> &keyMap, vec2 mouse_position);
	
	// Renders the player
	void draw(const mat3& projection)override;

	// Collision routines for turtles and fish
	bool collides_with(const Enemy& turtle);

	// Returns the current position
	vec2 get_position() const;

	// Returns the current rotation
	float get_rotation()const;

	// Moves the salmon's position by the specified offset
	void move(vec2 off);

	// Set salmon rotation in radians
	void set_rotation(float radians);

	// True if the salmon is alive
	bool is_alive()const;

	// Kills the salmon, changing its alive state and triggering on death events
	void lose_health(float amount);

	// Gain health after draining an enemy in vamp mode
    void gain_health(float amount);

    void reset_health();

	// Returns the bounding box for collision detection
	vec2 get_bounding_box() const;

	// Called when the salmon collides with an enemy, activate invulerability frames 
	void set_iframes(float magnitude);

	float get_iframes();

	int get_health() const;

	vec2 get_velocity() const;
	void set_velocity(vec2 vel);

	void set_acceleration(vec2 acc);

	void set_position(vec2 pos);

	void changeWeapon(Weapon* newWeapon);

    void set_vamp_expand(bool ex);

	bool get_vamp_expand();


private:
	float m_light_up_countdown_ms; // Used to keep track for how long the salmon should be lit up
	float m_iframe; // Used to indicate how long the player should be invulnerable for

	int m_init_health;

  	std::vector<Vertex> m_vertices;
	std::vector<uint16_t> m_indices;

	bool m_vamp_expand;

	Weapon* weapon;

};
