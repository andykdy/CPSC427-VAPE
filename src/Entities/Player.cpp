// Header
#include "Player.hpp"

// internal
#include "turtle.hpp"
#include "Entities/fish.hpp"

// stlib
#include <string>
#include <algorithm>
#include <cmath>

Texture Player::player_texture;
Texture Player::vamp_texture;

bool Player::init(vec2 screen, float health)
{
	m_vertices.clear();
	m_indices.clear();

	// Load shared texture
	if (!player_texture.is_valid())
	{
		if (!player_texture.load_from_file(textures_path("ship_normal.png")))
		{
			fprintf(stderr, "Failed to load the ship texture!");
			return false;
		}
	}

	// The position corresponds to the center of the texture
	float wr = player_texture.width * 0.5f;
	float hr = player_texture.height * 0.5f;

	TexturedVertex vertices[4];
	vertices[0].position = { -wr, +hr, -0.02f };
	vertices[0].texcoord = { 0.f, 1.f };
	vertices[1].position = { +wr, +hr, -0.02f };
	vertices[1].texcoord = { 1.f, 1.f };
	vertices[2].position = { +wr, -hr, -0.02f };
	vertices[2].texcoord = { 1.f, 0.f };
	vertices[3].position = { -wr, -hr, -0.02f };
	vertices[3].texcoord = { 0.f, 0.f };

	// Counterclockwise as it's the default opengl front winding direction
	uint16_t indices[] = { 0, 3, 1, 1, 3, 2 };
	// Clearing errors
	// Clearing errors
	gl_flush_errors();
	
	// Vertex Buffer creation
	glGenBuffers(1, &mesh.vbo);
	glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(TexturedVertex) * 4, vertices, GL_STATIC_DRAW);

	// Index Buffer creation
	glGenBuffers(1, &mesh.ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint16_t) * 6, indices, GL_STATIC_DRAW);

	// Vertex Array (Container for Vertex + Index buffer)
	glGenVertexArrays(1, &mesh.vao);
	if (gl_has_errors())
		return false;

	// Loading shaders
	if (!effect.load_from_file(shader_path("textured.vs.glsl"), shader_path("textured.fs.glsl")))
		return false;

	// Setting initial values
	motion.position = { screen.x / 2, screen.y - 100 };
	motion.radians = 0.f;
	motion.speed = 200.f;

	physics.scale = { -0.40, 0.40 };


	m_screen = screen;
	m_light_up_countdown_ms = -1.f;
	m_health = health;

	return true;
}

// Releases all graphics resources
void Player::destroy()
{
	glDeleteBuffers(1, &mesh.vbo);
	glDeleteBuffers(1, &mesh.ibo);
	glDeleteBuffers(1, &mesh.vao);

	glDeleteShader(effect.vertex);
	glDeleteShader(effect.fragment);
	glDeleteShader(effect.program);
}

// Called on each frame by World::update()
void Player::update(float ms, std::map<int, bool> &keyMap, vec2 mouse_position)
{
	float step = motion.speed * (ms / 1000);
	if (is_alive())
	{
		vec2 screenBuffer = { 20.0f,50.0f };
		float accelX = 0.f;
		float accelY = 0.f;

        // Arrow key movement;
        if (keyMap[GLFW_KEY_W]) accelY -= 1.f;
        if (keyMap[GLFW_KEY_S]) accelY += 1.f;
        if (keyMap[GLFW_KEY_A]) accelX -= 1.f;
        if (keyMap[GLFW_KEY_D]) accelX += 1.f;

        accelerate(accelX,accelY);

        // move based on velocity
		// std::clamp is not available, so using min max clamping instead 
        motion.position.x = std::min(std::max(motion.position.x + m_velocity.x, screenBuffer.x), m_screen.x - screenBuffer.x);
        motion.position.y = std::min(std::max(motion.position.y + m_velocity.y, screenBuffer.y), m_screen.y - screenBuffer.y);


        // Decay velocity
        float friction = 0.10;
        if (m_velocity.x > 0.f)
            m_velocity.x -= friction* m_velocity.x;
        else if (m_velocity.x < 0.f)
            m_velocity.x += -friction* m_velocity.x;

        if (m_velocity.y > 0.f)
            m_velocity.y -= friction*m_velocity.y;
        else if (m_velocity.y < 0.f)
            m_velocity.y += -friction*m_velocity.y;
	}
	else
	{
		// If dead we make it face upwards and sink deep down
		set_rotation(3.1415f);
		move({ 0.f, step });
	}

	if (m_light_up_countdown_ms > 0.f)
		m_light_up_countdown_ms -= ms;
	if (m_iframe > 0.f) {
		m_iframe -= ms;
	}
}

void Player::draw(const mat3& projection)
{
	transform.begin();
	transform.translate(motion.position);
	transform.scale(physics.scale);
	transform.rotate(motion.radians);
	transform.end();

	// Setting shaders
	glUseProgram(effect.program);

	// Enabling alpha channel for textures
	glEnable(GL_BLEND); glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDisable(GL_DEPTH_TEST);

	// Getting uniform locations for glUniform* calls
	GLint transform_uloc = glGetUniformLocation(effect.program, "transform");
	GLint color_uloc = glGetUniformLocation(effect.program, "fcolor");
	GLint projection_uloc = glGetUniformLocation(effect.program, "projection");

	// Setting vertices and indices
	glBindVertexArray(mesh.vao);
	glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.ibo);

	// Input data location as in the vertex buffer
	GLint in_position_loc = glGetAttribLocation(effect.program, "in_position");
	GLint in_texcoord_loc = glGetAttribLocation(effect.program, "in_texcoord");
	glEnableVertexAttribArray(in_position_loc);
	glEnableVertexAttribArray(in_texcoord_loc);
	glVertexAttribPointer(in_position_loc, 3, GL_FLOAT, GL_FALSE, sizeof(TexturedVertex), (void*)0);
	glVertexAttribPointer(in_texcoord_loc, 2, GL_FLOAT, GL_FALSE, sizeof(TexturedVertex), (void*)sizeof(vec3));

	// Enabling and binding texture to slot 0
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, player_texture.id);

	// Setting uniform values to the currently bound program
	glUniformMatrix3fv(transform_uloc, 1, GL_FALSE, (float*)&transform.out);
	float color[] = { 1.f, 1.f, 1.f };
	glUniform3fv(color_uloc, 1, color);
	glUniformMatrix3fv(projection_uloc, 1, GL_FALSE, (float*)&projection);

	// Drawing!
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, nullptr);
}

// Simple bounding box collision check
// This is a SUPER APPROXIMATE check that puts a circle around the bounding boxes and sees
// if the center point of either object is inside the other's bounding-box-circle. You don't
// need to try to use this technique.
bool Player::collides_with(const Turtle& turtle)
{
	float dx = motion.position.x - turtle.get_position().x;
	float dy = motion.position.y - turtle.get_position().y;
	float d_sq = dx * dx + dy * dy;
	float other_r = std::max(turtle.get_bounding_box().x, turtle.get_bounding_box().y);
	float my_r = std::max(physics.scale.x, physics.scale.y);
	float r = std::max(other_r, my_r);
	r *= 0.6f;
	if (d_sq < r * r)
		return true;
	return false;
}

bool Player::collides_with(const Fish& fish)
{
	float dx = motion.position.x - fish.get_position().x;
	float dy = motion.position.y - fish.get_position().y;
	float d_sq = dx * dx + dy * dy;
	float other_r = std::max(fish.get_bounding_box().x, fish.get_bounding_box().y);
	float my_r = std::max(physics.scale.x, physics.scale.y);
	float r = std::max(other_r, my_r);
	r *= 0.6f;
	if (d_sq < r * r)
		return true;
	return false;
}

vec2 Player::get_position() const
{
	return motion.position;
}

float Player::get_rotation() const {
    return motion.radians;
}

void Player::move(vec2 off)
{
	motion.position.x += off.x; 
	motion.position.y += off.y; 
}

void Player::set_rotation(float radians)
{
	motion.radians = radians;
}

void Player::accelerate(float x, float y) {
    float max = 4.f;

    float newX = m_velocity.x + x;
    if (newX > max) newX = max;
    if (newX < -max) newX = -max;

    float newY = m_velocity.y + y;
    if (newY > max) newY = max;
    if (newY < -max) newY = -max;

    m_velocity.x = newX;
    m_velocity.y = newY;
}

bool Player::is_alive() const
{
	return m_health > 0;
}

// Called when the player collides with an enemy
void Player::lose_health(float amount)
{
	m_health -= amount;
}

void Player::gain_health(float amount)
{
    m_health += amount;
}

// Called when the salmon collides with a fish
void Player::light_up()
{
	m_light_up_countdown_ms = 1500.f;
}

// Called when the player takes damage
void Player::set_iframes(float magnitude)
{
	m_iframe = magnitude;
}

float Player::get_iframes()
{
	return m_iframe;
}
