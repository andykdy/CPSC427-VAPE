// Header
#include "Player.hpp"

// internal
#include "turtle.hpp"
#include "Entities/fish.hpp"

// stlib
#include <string>
#include <algorithm>
#include <cmath>

bool Player::init(vec2 screen)
{
	m_vertices.clear();
	m_indices.clear();

	// Reads the salmon mesh from a file, which contains a list of vertices and indices
	FILE* mesh_file = fopen(mesh_path("salmon.mesh"), "r");
	if (mesh_file == nullptr)
		return false;

	// Reading vertices and colors
	size_t num_vertices;
	fscanf(mesh_file, "%zu\n", &num_vertices);
	for (size_t i = 0; i < num_vertices; ++i)
	{
		float x, y, z;
		float _u[3]; // unused
		int r, g, b;
		fscanf(mesh_file, "%f %f %f %f %f %f %d %d %d\n", &x, &y, &z, _u, _u+1, _u+2, &r, &g, &b);
		Vertex vertex;
		vertex.position = { x, y, -z }; 
		vertex.color = { (float)r / 255, (float)g / 255, (float)b / 255 };
		m_vertices.push_back(vertex);
	}

	// Reading associated indices
	size_t num_indices;
	fscanf(mesh_file, "%zu\n", &num_indices);
	for (size_t i = 0; i < num_indices; ++i)
	{
		int idx[3];
		fscanf(mesh_file, "%d %d %d\n", idx, idx + 1, idx + 2);
		m_indices.push_back((uint16_t)idx[0]);
		m_indices.push_back((uint16_t)idx[1]);
		m_indices.push_back((uint16_t)idx[2]);
	}

	// Done reading
	fclose(mesh_file);

	// Clearing errors
	gl_flush_errors();

	// Vertex Buffer creation
	glGenBuffers(1, &mesh.vbo);
	glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * m_vertices.size(), m_vertices.data(), GL_STATIC_DRAW);

	// Index Buffer creation
	glGenBuffers(1, &mesh.ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint16_t) * m_indices.size(), m_indices.data(), GL_STATIC_DRAW);

	// Vertex Array (Container for Vertex + Index buffer)
	glGenVertexArrays(1, &mesh.vao);
	if (gl_has_errors())
		return false;

	// Loading shaders
	if (!effect.load_from_file(shader_path("salmon.vs.glsl"), shader_path("salmon.fs.glsl")))
		return false;
	
	// Setting initial values
	motion.position = { screen.x / 2, screen.y - 100 };
	motion.radians = 1.5708;
	motion.speed = 200.f;

	physics.scale = { -35.f, 35.f };

	m_screen = screen;
	m_light_up_countdown_ms = -1.f;
	m_health = 3.0f;

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

	transform.translate({ motion.position.x, motion.position.y });
    transform.scale(physics.scale);
    transform.rotate(motion.radians);

	transform.end();

	// Setting shaders
	glUseProgram(effect.program);

	// Enabling alpha channel for textures
	glEnable(GL_BLEND); glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_DEPTH_TEST);

	// Getting uniform locations
	GLint transform_uloc = glGetUniformLocation(effect.program, "transform");
	GLint color_uloc = glGetUniformLocation(effect.program, "fcolor");
	GLint projection_uloc = glGetUniformLocation(effect.program, "projection");
	GLint light_up_uloc = glGetUniformLocation(effect.program, "light_up");

	// Setting vertices and indices
	glBindVertexArray(mesh.vao);
	glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.ibo);

	// Input data location as in the vertex buffer
	GLint in_position_loc = glGetAttribLocation(effect.program, "in_position");
	GLint in_color_loc = glGetAttribLocation(effect.program, "in_color");
	glEnableVertexAttribArray(in_position_loc);
	glEnableVertexAttribArray(in_color_loc);
	glVertexAttribPointer(in_position_loc, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	glVertexAttribPointer(in_color_loc, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)sizeof(vec3));

	// Setting uniform values to the currently bound program
	glUniformMatrix3fv(transform_uloc, 1, GL_FALSE, (float*)&transform.out);

	// !!! Player Color
	float color[] = { 1.f, 1.f, 1.f };
	if (m_iframe > 0.0f) {
		color[1] *= 2.0f;
	}
	glUniform3fv(color_uloc, 1, color);
	glUniformMatrix3fv(projection_uloc, 1, GL_FALSE, (float*)&projection);

	// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	// HERE TO SET THE CORRECTLY LIGHT UP THE SALMON IF HE HAS EATEN RECENTLY
	// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	int light_up = (m_light_up_countdown_ms > 0 ? 1 : 0);
	glUniform1iv(light_up_uloc, 1, &light_up);

	// Get number of infices from buffer,
	// we know our vbo contains both colour and position information, so...
	GLint size = 0;
	glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo);
	glGetBufferParameteriv(GL_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
	GLsizei num_indices = size / sizeof(uint16_t);

	// Drawing!
	glDrawElements(GL_TRIANGLES, num_indices, GL_UNSIGNED_SHORT, nullptr);
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
