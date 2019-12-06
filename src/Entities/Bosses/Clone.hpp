//
// Created by Andy Kim on 12/05/2019.
//

#ifndef VAPE_CLONE_HPP
#define VAPE_CLONE_HPP

#include <Engine/ECS/Entity.hpp>
#include <Entities/Projectiles and Damaging/Projectile.hpp>
#include <Entities/Player.hpp>
#include <Entities/Vamp.hpp>

class Projectile;
class Player;
class Vamp;

class Clone : public ECS::Entity {
public:
    virtual bool init(vec2 pos, vec2 disp) { return false; };
    virtual void update(float ms) {};
    virtual void draw(const mat3& projection) {};
    virtual void destroy() {};
    virtual void collideWith(const char* typeName, const Entity& other) {};
	virtual bool collidesWith(Player& player) = 0;
	virtual bool collidesWith(Vamp& vamp) = 0;
	virtual bool checkCollision(vec2 pos, vec2 box) const = 0;
    virtual void set_position(vec2 pos) {};
    virtual void set_velocity(vec2 vel) {};
    virtual vec2 get_position()const { return  { 0.f,0.f }; };
    virtual vec2 get_bounding_box()const { return  { 0.f,0.f }; };
    inline void add_vamp_timer(float ms) { vamp_timer += ms; };
    inline void reset_vamp_timer() { vamp_timer = 0; };
    inline float get_vamp_timer() { return vamp_timer; };
	virtual void stun() {};
	virtual void shutdown(float ms, vec2 master_pos) {};

    vec2 screen_size = {0, 0};
    float vamp_timer = 0;
	vec2 player_pos = { 0.f,0.f };
};

#endif //VAPE_CLONE_HPP
