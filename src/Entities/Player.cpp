// Header
#include "Player.hpp"

// stlib
#include <string>
#include <algorithm>
#include <cmath>

#include <Components/TransformComponent.hpp>
#include <Components/EffectComponent.hpp>
#include <Components/MotionComponent.hpp>
#include <Components/PhysicsComponent.hpp>
#include <Components/SpriteComponent.hpp>
#include <Components/BoundaryComponent.hpp>
#include <Components/HealthComponent.hpp>
#include <Engine/GameEngine.hpp>
#include <Entities/Weapons/BulletStraightShot.hpp>
#include "Entities/Weapons/WeaponTriShot.hpp"
#include <Components/PlayerComponent.hpp>
#include <Systems/ProjectileSystem.hpp>

// Same as static in c, local to compilation unit
namespace
{
	const size_t BULLET_COOLDOWN_MS = 300;
	const size_t spriteWH = 450;
	const size_t spriteFrames = 10;
	const vec2 screenBuffer = { 20, 50 };
}

Texture Player::player_texture;
Texture Player::vamp_texture;

bool Player::init(vec2 screen, int hp)
{
	auto* sprite = addComponent<SpriteComponent>();
	auto* effect = addComponent<EffectComponent>();
	auto* physics = addComponent<PhysicsComponent>();
	auto* motion = addComponent<MotionComponent>();
	auto* transform = addComponent<TransformComponent>();
	auto* player = addComponent<PlayerComponent>();
	auto* health = addComponent<HealthComponent>();

	addComponent<BoundaryComponent>(screenBuffer.x, screen.x - screenBuffer.x,
									screenBuffer.y, screen.y*0.9f - screenBuffer.y);

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
		throw std::runtime_error("Failed to initialize player sprite");

	// Setting initial values
	motion->position = { screen.x / 2, screen.y - 100 };
	motion->radians = 0.f;
	motion->maxVelocity = 400.f;
	motion->friction = 0.1;
	physics->scale = { -0.30, 0.30 };


	m_light_up_countdown_ms = -1.f;
	health->m_health = hp;
	m_iframe = 0.f;

	// weapon = new WeaponTriShot();
	weapon = new BulletStraightShot();
	weapon->init();

	return !gl_has_errors();

}

// Releases all graphics resources
void Player::destroy()
{
	weapon->destroy();

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

    weapon->update(ms);
	// Spawning player bullets
	if (is_alive() && keyMap[GLFW_KEY_SPACE]) {
		weapon->fire(motion->position, motion->radians + 3.14f);
	}

	if(weapon->amo < 0) {
	    weapon->destroy();
	    weapon = new BulletStraightShot();
	}

	if (is_alive())
	{
		float accelX = 0.f;
		float accelY = 0.f;

		float n = 100;
        // Arrow key movement;
        if (keyMap[GLFW_KEY_W]) accelY -= n;
        if (keyMap[GLFW_KEY_S]) accelY += n;
        if (keyMap[GLFW_KEY_A]) accelX -= n;
        if (keyMap[GLFW_KEY_D]) accelX += n;

        motion->acceleration = {accelX, accelY};
	}
	else
	{
		auto* boundary = getComponent<BoundaryComponent>();
		boundary->maxY = 2000;
		// If dead we make it face upwards and sink deep down
		set_rotation(3.1415f);
		motion->maxVelocity = 0;
		move({ 0.f, 200 * (ms / 1000) });
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

    transform->begin();
    transform->translate(motion->position);
    transform->scale(physics->scale);
    transform->rotate(motion->radians);
    transform->end();

	float mod = 1;
	if (m_iframe > 0)
		mod = 1/m_iframe;

	sprite->draw(projection, transform->out, effect->program, {1.f, mod * 1.f,mod * 1.f});
}

// Simple bounding box collision check
// This is a SUPER APPROXIMATE check that puts a circle around the bounding boxes and sees
// if the center point of either object is inside the other's bounding-box-circle. You don't
// need to try to use this technique.
bool Player::collides_with(const Enemy& enemy)
{
    auto* motion = getComponent<MotionComponent>();
    auto* physics = getComponent<PhysicsComponent>();

	float dx = motion->position.x - enemy.get_position().x;
	float dy = motion->position.y - enemy.get_position().y;
	float d_sq = dx * dx + dy * dy;
	float other_r = std::max(enemy.get_bounding_box().x, enemy.get_bounding_box().y);
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

bool Player::is_alive() const
{
	return getComponent<HealthComponent>()->is_alive();
}

// Called when the player collides with an enemy
void Player::lose_health(float amount)
{
	getComponent<HealthComponent>()->lose_health(amount);
}

void Player::gain_health(float amount)
{
	getComponent<HealthComponent>()->gain_health(amount);
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

int Player::get_health() const {
	return getComponent<HealthComponent>()->get_health();
}

void Player::changeWeapon(Weapon *newWeapon) {
	weapon->destroy();
	weapon = newWeapon;
	weapon->init();
}

vec2 Player::get_velocity() const {
	auto* motion = getComponent<MotionComponent>();
	return motion->velocity;
}

void Player::set_velocity(vec2 vel) {
	auto* motion = getComponent<MotionComponent>();
	motion->velocity = vel;
}

void Player::set_acceleration(vec2 acc) {
	auto* motion = getComponent<MotionComponent>();
	motion->acceleration = acc;
}

void Player::set_position(vec2 pos) {
	auto* motion = getComponent<MotionComponent>();
	motion->position = pos;
}

