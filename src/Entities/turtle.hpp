#pragma once

#include <Engine/ECS/Entity.hpp>
#include <Entities/Enemy.hpp>
#include "common.hpp"

// Player enemy
class Turtle : public Enemy{
	// Shared between all turtles, no need to load one for each instance
	static Texture turtle_texture;
public:

// Creates all the associated render resources and default transform
	bool init();

	// Releases all the associated resources
	void destroy() override;

	// Update turtle due to current
	// ms represents the number of milliseconds elapsed from the previous update() call
	void update(float ms) override;

	// Renders the salmon
	// projection is the 2D orthographic projection matrix
	void draw(const mat3& projection) override;

	// Returns the current turtle position
	vec2 get_position()const;

	// Sets the new turtle position
	void set_position(vec2 position);

	// Returns the turtle' bounding box for collision detection, called by collides_with()
	vec2 get_bounding_box() const;

	void set_velocity(vec2 velocity);
};