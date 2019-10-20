// Header
#include "Player.hpp"

// internal
#include "turtle.hpp"
#include "Entities/fish.hpp"

// stlib
#include <string>
#include <algorithm>
#include <cmath>
#include <Components/TransformComponent.hpp>
#include <Components/EffectComponent.hpp>
#include <Components/MotionComponent.hpp>
#include <Components/PhysicsComponent.hpp>
#include <Components/SpriteComponent.hpp>

// Same as static in c, local to compilation unit
namespace
{
	const size_t BULLET_COOLDOWN_MS = 300;
	const size_t spriteWH = 450;
	const size_t spriteFrames = 10;
}

Texture Player::player_texture;
Texture Player::vamp_texture;

bool Player::init(vec2 screen, float health)
{
	auto* sprite = addComponent<SpriteComponent>();
	auto* effect = addComponent<EffectComponent>();
	auto* physics = addComponent<PhysicsComponent>();
	auto* motion = addComponent<MotionComponent>();
	auto* transform = addComponent<TransformComponent>();

	// Load sound
	m_player_bullet_sound = Mix_LoadWAV(audio_path("pow.wav"));
	if ( m_player_bullet_sound == nullptr)
	{
		fprintf(stderr, "Failed to load sound player_bullet.wav\n %s\n %s\n %s\n make sure the data directory is present",
				audio_path("player_bullet.wav"));
		throw std::runtime_error("Failed to load sound player_bullet.wav");
	}

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

	// Loading shaders
	if (!effect->load_from_file(shader_path("textured.vs.glsl"), shader_path("textured.fs.glsl")))
		return false;

	if (!sprite->initTexture(&player_texture, spriteFrames, spriteWH, spriteWH))
		throw std::runtime_error("Failed to initialize health sprite");

	// Setting initial values
	motion->position = { screen.x / 2, screen.y - 100 };
	motion->radians = 0.f;
	motion->speed = 200.f;

	physics->scale = { -0.40, 0.40 };


	m_screen = screen;
	m_light_up_countdown_ms = -1.f;
	m_bullet_cooldown = -1.f;
	m_health = health;
	m_iframe = 0.f;

	if (gl_has_errors())
		return false;

	return true;
}

// Releases all graphics resources
void Player::destroy()
{
	if (m_player_bullet_sound != nullptr)
		Mix_FreeChunk(m_player_bullet_sound);

	for (auto& bullet : bullets)
		bullet.destroy();
	bullets.clear();

	auto* effect = getComponent<EffectComponent>();
	auto* sprite = getComponent<SpriteComponent>();

	effect->release();
	sprite->release();
	ECS::Entity::destroy();
}

// Called on each frame by World::update()
void Player::update(float ms, std::map<int, bool> &keyMap, vec2 mouse_position)
{
    auto* motion = getComponent<MotionComponent>();

	// Update player bullets
	for (auto& bullet : bullets)
		bullet.update(ms);

	// Spawning player bullets
	m_bullet_cooldown -= ms;
	if (is_alive() && keyMap[GLFW_KEY_SPACE] && m_bullet_cooldown < 0.f) {
		spawn_bullet();
		m_bullet_cooldown = BULLET_COOLDOWN_MS;
		Mix_PlayChannel(-1, m_player_bullet_sound, 0);
	}

	float step = motion->speed * (ms / 1000);
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

        accelerate(accelX,accelY); // TODO move velocity, acceleration, decay into movement

        // move based on velocity
		// std::clamp is not available, so using min max clamping instead 
        motion->position.x = std::min(std::max(motion->position.x + m_velocity.x, screenBuffer.x), m_screen.x - screenBuffer.x);
        motion->position.y = std::min(std::max(motion->position.y + m_velocity.y, screenBuffer.y), m_screen.y - screenBuffer.y);


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
    auto* transform = getComponent<TransformComponent>();
    auto* effect = getComponent<EffectComponent>();
    auto* motion = getComponent<MotionComponent>();
    auto* physics = getComponent<PhysicsComponent>();
    auto* sprite = getComponent<SpriteComponent>();

    // Draw player bullets
    for (auto& bullet : bullets)
        bullet.draw(projection);


    transform->begin();
    transform->translate(motion->position);
    transform->scale(physics->scale);
    transform->rotate(motion->radians);
    transform->end();

    sprite->draw(projection, transform->out, effect->program);
}

// TODO collisionSystem
// Simple bounding box collision check
// This is a SUPER APPROXIMATE check that puts a circle around the bounding boxes and sees
// if the center point of either object is inside the other's bounding-box-circle. You don't
// need to try to use this technique.
bool Player::collides_with(const Turtle& turtle)
{
    auto* motion = getComponent<MotionComponent>();
    auto* physics = getComponent<PhysicsComponent>();

	float dx = motion->position.x - turtle.get_position().x;
	float dy = motion->position.y - turtle.get_position().y;
	float d_sq = dx * dx + dy * dy;
	float other_r = std::max(turtle.get_bounding_box().x, turtle.get_bounding_box().y);
	float my_r = std::max(physics->scale.x, physics->scale.y);
	float r = std::max(other_r, my_r);
	r *= 0.6f;
	if (d_sq < r * r)
		return true;
	return false;
}

bool Player::collides_with(const Fish& fish)
{
    auto* motion = getComponent<MotionComponent>();
    auto* physics = getComponent<PhysicsComponent>();

    float dx = motion->position.x - fish.get_position().x;
	float dy = motion->position.y - fish.get_position().y;
	float d_sq = dx * dx + dy * dy;
	float other_r = std::max(fish.get_bounding_box().x, fish.get_bounding_box().y);
	float my_r = std::max(physics->scale.x, physics->scale.y);
	float r = std::max(other_r, my_r);
	r *= 0.6f;
	if (d_sq < r * r)
		return true;
	return false;
}

vec2 Player::get_position() const
{
    auto* motion = getComponent<MotionComponent>();

    return motion->position;
}

float Player::get_rotation() const {
    auto* motion = getComponent<MotionComponent>();
    return motion->radians;
}

void Player::move(vec2 off)
{
    auto* motion = getComponent<MotionComponent>();
	motion->position.x += off.x;
	motion->position.y += off.y;
}

void Player::set_rotation(float radians)
{
    auto* motion = getComponent<MotionComponent>();
	motion->radians = radians;
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

void Player::spawn_bullet() {
    auto* motion = getComponent<MotionComponent>();
	Bullet bullet;
	if (bullet.init(motion->position, motion->radians + 3.14)) {
		bullets.emplace_back(bullet);
	} else {
		throw std::runtime_error("Failed to spawn bullet");
	}
}

vec2 Player::get_bounding_box() const {
    auto* physics = getComponent<PhysicsComponent>();

    // fabs is to avoid negative scale due to the facing direction
    return { std::fabs(physics->scale.x) * spriteWH, std::fabs(physics->scale.y) * spriteWH };
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

float Player::get_health() const {
    return m_health;
}

Player::Player(ECS::EntityId id) : Entity(id) {}
