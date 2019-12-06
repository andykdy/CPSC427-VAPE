#pragma once

#include <Entities/Enemies/Enemy.hpp>
#include "common.hpp"
#include <random>

// Player enemy
class PickupEnemy : public Enemy{
	static Texture enemy_texture;
public:

// Creates all the associated render resources and default transform
	bool init() override;

	// Releases all the associated resources
	void destroy() override;

	// Update turtle due to current
	// ms represents the number of milliseconds elapsed from the previous update() call
	void update(float ms) override;

	// Renders the salmon
	// projection is the 2D orthographic projection matrix
	void draw(const mat3& projection) override;

	// Returns the current position
	vec2 get_position()const override;

	// Sets the new position
	void set_position(vec2 position) override;

	vec2 get_bounding_box() const override;

	void set_velocity(vec2 velocity) override;

	void spawn_pickup();

private:
	std::default_random_engine m_rand;
};