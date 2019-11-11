// Header
#include "Entities/Enemies/turtle.hpp"

#include <cmath>
#include <Components/SpriteComponent.hpp>
#include <Components/EffectComponent.hpp>
#include <Components/PhysicsComponent.hpp>
#include <Components/MotionComponent.hpp>
#include <Components/TransformComponent.hpp>

Texture Turtle::turtle_texture;

bool Turtle::init()
{
	auto* sprite = addComponent<SpriteComponent>();
	auto* effect = addComponent<EffectComponent>();
	auto* physics = addComponent<PhysicsComponent>();
	auto* motion = addComponent<MotionComponent>();
	auto* transform = addComponent<TransformComponent>();

	// Load shared texture
	if (!turtle_texture.is_valid())
	{
		if (!turtle_texture.load_from_file(textures_path("turtle.png")))
		{
			fprintf(stderr, "Failed to load turtle texture!");
			return false;
		}
	}

	// Loading shaders
	if (!effect->load_from_file(shader_path("textured.vs.glsl"), shader_path("textured.fs.glsl")))
		return false;


	if (!sprite->initTexture(&turtle_texture))
		throw std::runtime_error("Failed to initialize health sprite");

	if (gl_has_errors())
		return false;

	motion->radians = (rand() % 4) * 3.14f / 4;
	motion->velocity = {0.f, 180.f};

	// Setting initial values, scale is negative to make it face the opposite way
	// 1.0 would be as big as the original texture.
	physics->scale = { -0.33f, 0.33f };

	return true;
}

// Releases all graphics resources
void Turtle::destroy()
{
	auto* effect = getComponent<EffectComponent>();
	auto* sprite = getComponent<SpriteComponent>();

	effect->release();
	sprite->release();
	ECS::Entity::destroy();
}

void Turtle::update(float ms)
{
	auto* motion = getComponent<MotionComponent>();

	float step = 180.f * (ms / 1000);
	motion->radians += step * 0.02;
}

void Turtle::draw(const mat3& projection)
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

	sprite->draw(projection, transform->out, effect->program);
}

vec2 Turtle::get_position()const
{
	auto* motion = getComponent<MotionComponent>();
	return motion->position;
}

void Turtle::set_position(vec2 position)
{
	auto* motion = getComponent<MotionComponent>();
	motion->position = position;
}

vec2 Turtle::get_bounding_box() const
{
	auto* physics = getComponent<PhysicsComponent>();

	// Returns the local bounding coordinates scaled by the current size of the turtle 
	// fabs is to avoid negative scale due to the facing direction.
	return { std::fabs(physics->scale.x) * turtle_texture.width, std::fabs(physics->scale.y) * turtle_texture.height };
}

void Turtle::set_velocity(vec2 velocity) {
	auto* motion = getComponent<MotionComponent>();
	motion->velocity.x = velocity.x;
	motion->velocity.y = velocity.y;
}
