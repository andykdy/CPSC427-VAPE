#pragma once

#include <map>
#include "common.hpp"
#include <vector>

class Turtle;
class Fish;

class Player : public Entity
{
	static Texture player_texture;
	static Texture vamp_texture;
public:
	// Creates all the associated render resources and default transform
	bool init(vec2 screen);

	// Releases all associated resources
	void destroy();
	
	// Update salmon position based on direction
	// ms represents the number of milliseconds elapsed from the previous update() call
	void update(float ms, std::map<int, bool> &keyMap, vec2 mouse_position);
	
	// Renders the salmon
	void draw(const mat3& projection)override;

	// Collision routines for turtles and fish
	bool collides_with(const Turtle& turtle);
	bool collides_with(const Fish& fish);

	// Returns the current salmon position
	vec2 get_position() const;

	// Returns the current salmon rotation
	float get_rotation()const;

	// Moves the salmon's position by the specified offset
	void move(vec2 off);

	// Set salmon rotation in radians
	void set_rotation(float radians);

	// Change salmon velocity
	void accelerate(float x, float y);

	// True if the salmon is alive
	bool is_alive()const;

	// Kills the salmon, changing its alive state and triggering on death events
	void lose_health(float amount);

	// Called when the salmon collides with a fish, starts lighting up the salmon
	void light_up();

	// Called when the salmon collides with an enemy, activate invulerability frames 
	void set_iframes(float magnitude);

	float get_iframes();

private:
	float m_light_up_countdown_ms; // Used to keep track for how long the salmon should be lit up
	float m_iframe; // Used to indicate how long the player should be invulnerable for
	float m_health;
	vec2 m_velocity; // Velocity
	vec2 m_screen; // Screen space

  	std::vector<Vertex> m_vertices;
	std::vector<uint16_t> m_indices;
};
