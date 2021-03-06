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
    inline void add_vamp_timer(float ms) { vamp_timer += ms; };
    inline void reset_vamp_timer() { vamp_timer = 0; };
    inline float get_vamp_timer() { return vamp_timer; };
    inline int get_points() { return points; };
    virtual void set_path(std::vector<vec2> path) {};
    virtual std::vector<vec2> get_path() {return std::vector<vec2>();};

    vec2 screen_size = {0, 0};
    vec2 player_position = {0,0,};
    float vamp_timer = 0;
    int points = 0;
};

#endif //VAPE_PROJECTILE_HPP
