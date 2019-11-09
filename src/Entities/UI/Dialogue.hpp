#pragma once

#include "common.hpp"

class Dialogue : public EntityOld
{
	static Texture current_texture;

public:
	// Creates all the associated render resources and default transform
	bool init(const char *path);

	// Releases all the associated resources
	void destroy();

	void update(float ms);

	void draw(const mat3& projection) override;

	vec2 get_position()const;

	void set_position(vec2 position);

	bool isActive();

	void deactivate();
	void activate();

	// Move to next dialogue option
	void next();

private: 
	bool m_active;
	int m_index;
};