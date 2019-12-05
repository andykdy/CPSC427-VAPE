// Header
#include "Entities/Enemies/PickupEnemy.hpp"

#include <Engine/GameEngine.hpp>
#include <cmath>
#include <random>
#include <Components/SpriteComponent.hpp>
#include <Components/EffectComponent.hpp>
#include <Components/PhysicsComponent.hpp>
#include <Components/MotionComponent.hpp>
#include <Components/TransformComponent.hpp>
#include <Components/EnemyComponent.hpp>
#include <Entities/Pickups/Pickup.hpp>
#include <Entities/Pickups/TriShotPickup.hpp>
#include <Entities/Pickups/MachineGunPickup.hpp>
#include <Systems/PickupSystem.hpp>


Texture PickupEnemy::enemy_texture;

namespace {
    const size_t POINTS_VAL = 50;
}

bool PickupEnemy::init()
{
	auto* sprite = addComponent<SpriteComponent>();
	auto* effect = addComponent<EffectComponent>();
	auto* physics = addComponent<PhysicsComponent>();
	auto* motion = addComponent<MotionComponent>();
	auto* transform = addComponent<TransformComponent>();
	auto* enemy = addComponent<EnemyComponent>();

	// Load shared texture
	if (!enemy_texture.is_valid())
	{
		if (!enemy_texture.load_from_file(textures_path("pickup_enemy.png")))
		{
			fprintf(stderr, "Failed to load pickup enemy texture!");
			return false;
		}
	}

	// Loading shaders
	if (!effect->load_from_file(shader_path("textured.vs.glsl"), shader_path("textured.fs.glsl")))
		return false;


	if (!sprite->initTexture(&enemy_texture))
		throw std::runtime_error("Failed to initialize turtle sprite");

	if (gl_has_errors())
		return false;

	motion->radians = (rand() % 4) * 3.14f / 4;
	motion->velocity = {0.f, 90.f};

	// Setting initial values, scale is negative to make it face the opposite way
	// 1.0 would be as big as the original texture.
	physics->scale = { -0.14f, 0.14f };
	points = POINTS_VAL;

	return true;
}

// Releases all graphics resources
void PickupEnemy::destroy()
{
	auto* effect = getComponent<EffectComponent>();
	auto* sprite = getComponent<SpriteComponent>();
	spawn_pickup();
	

	effect->release();
	sprite->release();
	ECS::Entity::destroy();
}

void PickupEnemy::update(float ms)
{
	auto* motion = getComponent<MotionComponent>();

	float step = 180.f * (ms / 1000);
	motion->radians += step * 0.02;
}

void PickupEnemy::draw(const mat3& projection)
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

vec2 PickupEnemy::get_position()const
{
	auto* motion = getComponent<MotionComponent>();
	return motion->position;
}

void PickupEnemy::set_position(vec2 position)
{
	auto* motion = getComponent<MotionComponent>();
	motion->position = position;
}

vec2 PickupEnemy::get_bounding_box() const
{
	auto* physics = getComponent<PhysicsComponent>();

	// Returns the local bounding coordinates scaled by the current size of the turtle 
	// fabs is to avoid negative scale due to the facing direction.
	return { std::fabs(physics->scale.x) * enemy_texture.width, std::fabs(physics->scale.y) * enemy_texture.height };
}

void PickupEnemy::set_velocity(vec2 velocity) {
	auto* motion = getComponent<MotionComponent>();
	motion->velocity.x = velocity.x;
	motion->velocity.y = velocity.y;
}

void PickupEnemy::spawn_pickup() {
	auto* motion = getComponent<MotionComponent>();
	auto& ps = GameEngine::getInstance().getSystemManager()->getSystem<PickupSystem>();

	ECS::EntityManager* e = GameEngine::getInstance().getEntityManager();
	auto* p = &e->addEntity<MachineGunPickup>();
		
	p->init(motion->position);
	ps.pickups.emplace_back(p);
}
