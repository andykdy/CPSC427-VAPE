//
// Created by Cody on 11/10/2019.
//

#include "TestPickup.hpp"

#include <cmath>
#include <Components/SpriteComponent.hpp>
#include <Components/EffectComponent.hpp>
#include <Components/PhysicsComponent.hpp>
#include <Components/MotionComponent.hpp>
#include <Components/TransformComponent.hpp>
#include <Components/EnemyComponent.hpp>

Texture TestPickup::pickup_texture;

bool TestPickup::init(vec2 position) {
	auto* sprite = addComponent<SpriteComponent>();
	auto* effect = addComponent<EffectComponent>();
	auto* physics = addComponent<PhysicsComponent>();
	auto* motion = addComponent<MotionComponent>();
	auto* transform = addComponent<TransformComponent>();
	auto* enemy = addComponent<EnemyComponent>();

	// Load shared texture
	if (!pickup_texture.is_valid())
	{
		if (!pickup_texture.load_from_file(textures_path("pickup.png")))
		{
			fprintf(stderr, "Failed to load pickup enemy texture!");
			return false;
		}
	}

	// Loading shaders
	if (!effect->load_from_file(shader_path("textured.vs.glsl"), shader_path("textured.fs.glsl")))
		return false;


	if (!sprite->initTexture(&pickup_texture))
		throw std::runtime_error("Failed to initialize turtle sprite");

	if (gl_has_errors())
		return false;

	motion->radians = (rand() % 4) * 3.14f / 4;
	motion->velocity = { 0.f, 90.f };
	motion->position = position;

	// Setting initial values, scale is negative to make it face the opposite way
	// 1.0 would be as big as the original texture.
	physics->scale = { -0.28f, 0.28f };
}

void TestPickup::update(float ms) {
	auto* motion = getComponent<MotionComponent>();

	float step = 180.f * (ms / 1000);
	motion->radians += step * 0.02;
}

void TestPickup::draw(const mat3 &projection) {
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

	sprite->draw(projection, transform->out, effect->program);
}

void TestPickup::destroy() {
	auto* effect = getComponent<EffectComponent>();
	auto* sprite = getComponent<SpriteComponent>();

	effect->release();
	sprite->release();
	ECS::Entity::destroy();
}

vec2 TestPickup::get_position() const {
    return vec2();
}

bool TestPickup::collides_with(const Player &player) {
    return false;
}

void TestPickup::applyEffect(LevelState& level) {

}

bool TestPickup::isOffScreen(const vec2 &screen) {
    return false;
}
