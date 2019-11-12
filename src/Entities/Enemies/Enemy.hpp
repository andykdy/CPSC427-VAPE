#ifndef VAPE_ENEMY_HPP
#define VAPE_ENEMY_HPP

#include <Engine/ECS/Entity.hpp>
#include <Entities/Projectiles and Damaging/Projectile.hpp>

class Projectile;

class Enemy : public ECS::Entity {
public:
	virtual bool init() { return false; };
	virtual void update(float ms) {};
	virtual void draw(const mat3& projection) {};
	virtual void destroy() {};
	virtual void collideWith(const char* typeName, const Entity& other) {};
	virtual void set_position(vec2 pos) {};
	virtual void set_velocity(vec2 vel) {};
	virtual vec2 get_position()const { return  { 0.f,0.f }; };
	virtual vec2 get_bounding_box()const { return  { 0.f,0.f }; };

    std::vector<Projectile *> projectiles;
};

#endif //VAPE_PROJECTILE_HPP
