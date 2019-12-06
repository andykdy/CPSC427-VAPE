//
// Created by Andy Kim on 12/05/2019.
//

#include "Boss3.hpp"
#include <cmath>
#include <iostream>
#include <Components/SpriteComponent.hpp>
#include <Components/EffectComponent.hpp>
#include <Components/PhysicsComponent.hpp>
#include <Components/MotionComponent.hpp>
#include <Components/TransformComponent.hpp>

namespace
{
	const size_t BURST_COOLDOWN_MS = 800;
	const size_t BULLET_COOLDOWN_MS = 100;
	const size_t DAMAGE_EFFECT_TIME = 100;
	const size_t DIRECTION_COOLDOWN_MS = 800;
	const size_t BULLET_DAMAGE = 5;
	const size_t INIT_HEALTH = 150;
	const size_t POINTS_VAL = 5000;
	const size_t ROAM_VELOCITY = 120;
	const size_t COOLDOWN_TIME_MS = 1000;
	const size_t CHARGE_TIME_MS = 4000;
	const size_t SCREEN_BUFFER = 100;
}


Texture Boss3::boss3_texture;

bool Boss3::init(vec2 screen) {
	m_healthbar = &GameEngine::getInstance().getEntityManager()->addEntity<BossHealth>();
	m_healthbar->init(screen, INIT_HEALTH);

	gl_flush_errors();
	auto* sprite = addComponent<SpriteComponent>();
	auto* effect = addComponent<EffectComponent>();
	auto* physics = addComponent<PhysicsComponent>();
	auto* motion = addComponent<MotionComponent>();
	auto* transform = addComponent<TransformComponent>();

	// Load shared texture
	if (!boss3_texture.is_valid())
	{
		if (!boss3_texture.load_from_file(textures_path("boss3.png")))
		{
			fprintf(stderr, "Failed to load Boss1 texture!");
			return false;
		}
	}

	// Loading shaders
	if (!effect->load_from_file(shader_path("textured.vs.glsl"), shader_path("textured.fs.glsl")))
		return false;

	if (!sprite->initTexture(&boss3_texture))
		throw std::runtime_error("Failed to initialize bullet sprite");

	if (gl_has_errors())
		return false;
	motion->position = { screen.x/2, 200.f };
	motion->radians = 3.14;
	motion->velocity = { 0.f, 0.f };
	m_screen = screen;

	// Setting initial values, scale is negative to make it face the opposite way
	// 1.0 would be as big as the original texture.
	physics->scale = { -0.35f, 0.35f };

	health = INIT_HEALTH;
	m_is_alive = true;
	m_is_cloned = false;
	points = POINTS_VAL;
	m_cooldown_timer = COOLDOWN_TIME_MS;
	m_charge_timer = CHARGE_TIME_MS;
	m_target = { 0.f,0.f };
	m_curr_state = Boss3State::aiming;

	return true;
}

void Boss3::destroy() {
	m_healthbar->destroy();

	for (auto bullet : projectiles)
		bullet->destroy();
	projectiles.clear();

	for (auto clone : clones)
		clone->destroy();
	clones.clear();

	auto* effect = getComponent<EffectComponent>();
	auto* sprite = getComponent<SpriteComponent>();

	effect->release();
	sprite->release();
	ECS::Entity::destroy();
}

void Boss3::update(float ms) {
	auto* motion = getComponent<MotionComponent>();
	m_healthbar->setHealth(health);
	for (auto clone : clones) {
		clone->update(ms);
		clone->player_pos = player_position;
	}
	if (m_damage_effect_cooldown > 0)
		m_damage_effect_cooldown -= ms;

	// Update bullets
	for (auto bullet : projectiles)
		bullet->update(ms);

	// Simple health based states, only two states for this first boss
	if (health > 90) state1Update(ms);
	else if (health > 0) state2Update(ms);
	else {
		motion->velocity = { 0.f,0.f };
		auto clone_it = clones.begin();
		while (clone_it != clones.end()) {
			(*clone_it)->shutdown(ms, motion->position);
			clone_it++;
		}
	}
}

void Boss3::state1Update(float ms) {
	auto* motion = getComponent<MotionComponent>();

	float dy = player_position.y - motion->position.y;
	float dx = player_position.x - motion->position.x;
	if (m_curr_state == Boss3State::aiming) {
		if (m_cooldown_timer > 0) {
			float angle = atan2(dx, dy);
			motion->radians = angle - M_PI;
			m_cooldown_timer -= ms;
		}
		else {
			m_cooldown_timer = COOLDOWN_TIME_MS;
			m_target = { dx,dy };
			m_curr_state = Boss3State::charging;
		}
	}
	else if (m_curr_state == Boss3State::charging) {

		if (m_charge_timer < 0 || nearBounds()) {
			m_charge_timer = CHARGE_TIME_MS;
			motion->velocity = { 0.f,0.f };
			m_curr_state = Boss3State::cooldown;
		} else  {
			float angle = atan2(m_target.y, m_target.x);
			motion->velocity.x = 360.f * cos(angle);
			motion->velocity.y = 360.f * sin(angle);
			m_charge_timer -= ms;
		}
	}
	else if (m_curr_state == Boss3State::cooldown) {
		motion->velocity = { (m_screen.x/2.f - motion->position.x)/15.f, (m_screen.x / 2.f - motion->position.y)/15.f };
		if (m_cooldown_timer > 0) {
			m_cooldown_timer -= ms;
		}
		else {
			m_cooldown_timer = COOLDOWN_TIME_MS;
			m_curr_state = Boss3State::aiming;
		}
	}
}

void Boss3::state2Update(float ms) {
	auto* motion = getComponent<MotionComponent>();
	if (motion->position.y > 200.f) {
		motion->velocity = { 0.f, ms * -15.f };
		motion->radians = 0.f;
	}
	else {
		if (!m_is_cloned) {
			spawnClones();
			m_is_cloned = true;
			motion->velocity.x = ms * ROAM_VELOCITY / 10.f;
			motion->velocity.y = 0.f;
			motion->radians = 3 * M_PI / 2;
		}
		else {
			if (motion->position.x < 100.f) {
				motion->velocity.x = ms * ROAM_VELOCITY / 10.f;
				motion->radians = 3 * M_PI / 2;
			}
			else if (motion->position.x > m_screen.x - 100.f) {
				motion->velocity.x = -1.f * ms * ROAM_VELOCITY / 10.f;
				motion->radians = M_PI / 2;
			}
		}
	}
}

void Boss3::spawnClones() {
	auto* motion = getComponent<MotionComponent>();
	vec2 master_pos = motion->position;
	
	// One clone is leader, will do different things than the other
	Boss3Clone* clone1 = &GameEngine::getInstance().getEntityManager()->addEntity<Boss3Clone>();
	clone1->init(master_pos, { master_pos.x - 100.f, master_pos.y + 100.f });
	clone1->set_lead();

	Boss3Clone* clone2 = &GameEngine::getInstance().getEntityManager()->addEntity<Boss3Clone>();
	clone2->init(master_pos, { master_pos.x + 100.f, master_pos.y + 100.f });
	clones.emplace_back(clone1);
	clones.emplace_back(clone2);
}

void Boss3::draw(const mat3 &projection) {
	// Draw boss' bullets
	for (auto bullet : projectiles)
		bullet->draw(projection);
	if (m_is_cloned) {
		for (auto clone : clones)
			clone->draw(projection);
	}

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
	if (m_damage_effect_cooldown > 0)
		mod = 1 / m_damage_effect_cooldown;

	sprite->draw(projection, transform->out, effect->program, { 1.f, mod * 1.f,mod * 1.f });

	m_healthbar->draw(projection);
}

vec2 Boss3::get_position() const {
	auto* motion = getComponent<MotionComponent>();
	return motion->position;
}

void Boss3::set_position(vec2 position) {
	auto* motion = getComponent<MotionComponent>();
	motion->position = position;
}

vec2 Boss3::get_bounding_box() const {
	auto* physics = getComponent<PhysicsComponent>();

	return { std::fabs(physics->scale.x) * boss3_texture.width * 0.6f, std::fabs(physics->scale.y) * boss3_texture.height * 0.6f };
}

void Boss3::addDamage(int damage) {
	m_damage_effect_cooldown = DAMAGE_EFFECT_TIME;
	Boss::addDamage(damage);
}

bool Boss3::collidesWith(const Vamp &vamp) {
    return checkCollision(vamp.get_position(), vamp.get_bounding_box());
}

bool Boss3::collidesWith(Player &player) {
	auto* motion = getComponent<MotionComponent>();

	vec2 playerbox = player.get_bounding_box();

	bool collides = checkCollision(player.get_position(), playerbox);
	if (collides) {
		vec2 player_pos = player.get_position();
		float player_r = std::max(playerbox.x, playerbox.y) * 0.4f;
		float boss_r = std::max(get_bounding_box().x, get_bounding_box().y) * 0.4f;

		auto dist = std::sqrt((float)pow(player_pos.x - motion->position.x, 2) + (float)pow(player_pos.y - motion->position.y, 2));
		float overlap = (dist - player_r - boss_r);
		float dx = (player_pos.x - motion->position.x) / dist;
		float dy = (player_pos.y - motion->position.y) / dist;
		player_pos.x -= overlap * dx;
		player_pos.y -= overlap * dy;
		player.set_position(player_pos);


		float boss_mass = boss_r * 200;
		float player_mass = player_r * 100;

		vec2 player_vel = player.get_velocity();
		vec2 boss_vel = motion->velocity;
		
		float vbp1 = 2 * boss_mass / (player_mass + boss_mass);
		float vbp2 = dot(sub(player_vel, boss_vel), sub(player_pos, motion->position)) / (float)pow(len(sub(player_pos, motion->position)), 2);
		vec2 vb = sub(player_vel, mul(sub(player_pos, motion->position), vbp1 * vbp2));

		player.set_velocity(vb);
		player.set_acceleration({ 0,0 });
	}
	return collides;
}

bool Boss3::checkCollision(vec2 pos, vec2 box) const {
	auto* motion = getComponent<MotionComponent>();
	auto* physics = getComponent<PhysicsComponent>();

	vec2 bbox = this->get_bounding_box();

	float dx = motion->position.x - pos.x;
	float dy = motion->position.y - pos.y;
	float d_sq = dx * dx + dy * dy;
	float dr = (std::max(bbox.x, bbox.y) * 0.4f) + (std::max(box.x, box.y) * 0.4f);
	float r_sq = dr * dr;
	return d_sq < r_sq;
}

bool Boss3::nearBounds() {
	auto* motion = getComponent<MotionComponent>();
	return (motion->position.x < SCREEN_BUFFER) || (motion->position.y < SCREEN_BUFFER) ||
		(motion->position.x > m_screen.x - SCREEN_BUFFER) || (motion->position.y > m_screen.y - SCREEN_BUFFER);
}
