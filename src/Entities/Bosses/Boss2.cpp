//
// Created by Cody on 11/7/2019.
//

#include <Engine/GameEngine.hpp>
#include <Components/SpriteComponent.hpp>
#include <Components/EffectComponent.hpp>
#include <Components/PhysicsComponent.hpp>
#include <Components/MotionComponent.hpp>
#include <Components/TransformComponent.hpp>
#include <Entities/Projectiles and Damaging/Laser.hpp>
#include "Boss2.hpp"


// Same as static in c, local to compilation unit
namespace
{
    const size_t INIT_HEALTH = 100;
    const size_t SPRITE_FRAMES = 4;
    const size_t SPRITE_W = 264;
    const size_t SPRITE_H = 88;
    const size_t DAMAGE_EFFECT_TIME = 100;

    const std::vector<vec2> hardpoints = {
            {44,76},
            {76,72},
                    // TODO bullet hardpoints too?
            {123,85},
            {140,85},

            {187,72},
            {219,76}
    };

    const vec2 SPRITE_SCALE = {3.5, 3.5f};
    const vec2 MESH_SCALE = {300.f, 300.f};
}

Texture Boss2::boss2_texture;

bool Boss2::init(vec2 screen) {
    m_healthbar = &GameEngine::getInstance().getEntityManager()->addEntity<BossHealth>();
    m_healthbar->init(screen, INIT_HEALTH);

    m_vertices.clear();

    gl_flush_errors();
    auto* sprite = addComponent<SpriteComponent>();
    auto* effect = addComponent<EffectComponent>();
    auto* physics = addComponent<PhysicsComponent>();
    auto* motion = addComponent<MotionComponent>();
    auto* transform = addComponent<TransformComponent>();

    // Load shared texture
    if (!boss2_texture.is_valid())
    {
        if (!boss2_texture.load_from_file(textures_path("boss2.png")))
        {
            fprintf(stderr, "Failed to load Boss1 texture!");
            return false;
        }
    }

    // Loading shaders
    if (!effect->load_from_file(shader_path("textured.vs.glsl"), shader_path("textured.fs.glsl")))
        return false;

    if (!sprite->initTexture(&boss2_texture, SPRITE_FRAMES, SPRITE_W, SPRITE_H))
        throw std::runtime_error("Failed to initialize boss2 sprite");

    if (gl_has_errors())
        return false;

    // Collision Mesh
    FILE* mesh_file = fopen(mesh_path("boss2.testmesh"), "r");
    size_t num_vertices;
    fscanf(mesh_file, "%zu\n", &num_vertices);
    for (size_t i = 0; i < num_vertices; ++i)
    {
        float x, y;
        fscanf(mesh_file, "%f %f\n", &x, &y);
        Vertex vertex;
        vertex.position = { x, y, -0.01f };
        vertex.color = { 1.f,1.f,1.f };
        m_vertices.push_back(vertex);
    }
    m_dot.init();


    motion->position = {0.f, 0.f };
    motion->radians = 0;
    motion->velocity = {0.f, 0.f};

    // Setting initial values, scale is negative to make it face the opposite way
    // 1.0 would be as big as the original texture.
    physics->scale = SPRITE_SCALE;

    health = INIT_HEALTH;
    m_is_alive = true;

    // Initialize lasers
    for (vec2 hardpoint : hardpoints) {
        Laser* laser = &GameEngine::getInstance().getEntityManager()->addEntity<Laser>();
        laser->init({0,0}, 0);
        laser->setState(laserState::firing); // TODO remove

        // In projectiles for levelstate to use, but also in m_lasers for us to do laser-specific functions
        projectiles.push_back(laser);
        m_lasers.push_back(laser);
    }

    return true;
}

void Boss2::destroy() {
    m_healthbar->destroy();
    m_dot.destroy();

    for (auto laser : projectiles)
        laser->destroy();
    projectiles.clear();

    auto* effect = getComponent<EffectComponent>();
    auto* sprite = getComponent<SpriteComponent>();

    effect->release();
    sprite->release();
    ECS::Entity::destroy();
}

void Boss2::update(float ms) {
    auto* motion = getComponent<MotionComponent>();
    auto* physics = getComponent<PhysicsComponent>();

    m_healthbar->setHealth(health);
    if (m_damage_effect_cooldown > 0)
        m_damage_effect_cooldown -= ms;

    // Update lasers
    vec2 bbox = get_bounding_box();
    float left = motion->position.x - bbox.x/2;
    float top = motion->position.y - bbox.y/2;
    for (int i = 0; i < hardpoints.size(); i++) {
        std::cout << left << "," << top << std::endl;
        std::cout << hardpoints[i].x*physics->scale.x << "," << hardpoints[i].y*physics->scale.y << std::endl << std::endl;
        m_lasers[i]->set_position({hardpoints[i].x*physics->scale.x  + left, hardpoints[i].y*physics->scale.y + top});
    }
    for (auto laser : projectiles) {
        laser->update(ms);
    }

    // TODO behavior - laser patterns
}

void Boss2::draw(const mat3 &projection) {
    // Draw boss' bullets
    for (auto laser : projectiles)
        laser->draw(projection);

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
        mod = 1/m_damage_effect_cooldown;

    sprite->draw(projection, transform->out, effect->program, {1.f, mod * 1.f,mod * 1.f});

    // Vertex Debug Drawing
    for (auto& vertex : m_vertices) {
        transform->begin();
        transform->translate(motion->position);
        transform->scale(MESH_SCALE);
        transform->rotate(motion->radians + 1.5708f);
        transform->end();

        vec3 pos = mul(transform->out, vec3{vertex.position.x, vertex.position.y, 1.0});
        m_dot.draw(projection, {1.f,1.f,1.f}, {pos.x, pos.y}, 0);
    }

    m_healthbar->draw(projection);
}

vec2 Boss2::get_position() const {
    auto* motion = getComponent<MotionComponent>();
    return motion->position;
}

void Boss2::set_position(vec2 position) {
    auto* motion = getComponent<MotionComponent>();
    motion->position = {position.x, position.y};
}

vec2 Boss2::get_bounding_box() const {
    auto* physics = getComponent<PhysicsComponent>();
    // Returns the local bounding coordinates scaled by the current size of the turtle
    // fabs is to avoid negative scale due to the facing direction.
    return { std::fabs(physics->scale.x) * SPRITE_W, std::fabs(physics->scale.y) * SPRITE_H };
}

void Boss2::addDamage(int damage) {
    m_damage_effect_cooldown = DAMAGE_EFFECT_TIME;
    Boss::addDamage(damage);
}

bool Boss2::collidesWith(const Vamp& vamp) {
    return checkCollision(vamp.get_position(), vamp.get_bounding_box());
}

bool Boss2::collidesWith(const Player &player) {
    return checkCollision(player.get_position(), player.get_bounding_box());
}

bool Boss2::checkCollision(vec2 pos, vec2 box) const {
    auto* motion = getComponent<MotionComponent>();
    auto* physics = getComponent<PhysicsComponent>();

    // bounding box slightly larger than sprite
    vec2 bbox = { (std::fabs(physics->scale.x)+0.1f) * SPRITE_W, (std::fabs(physics->scale.y)+0.1f) * SPRITE_H };

    // AABB Bounding box check first
    if ( !((fabs(motion->position.x - pos.x) * 2 < (bbox.x + box.x)) &&
         (fabs(motion->position.y - pos.y) * 2 < (bbox.y + box.y)))) {
         return false;
    }

    auto* transform = getComponent<TransformComponent>();
    // For each vertex, check if within the area
    for(auto vertex : m_vertices) {
        transform->begin();
        transform->translate(motion->position);
        transform->scale(MESH_SCALE);
        transform->rotate(motion->radians + 1.5708f);
        transform->end();
        vec3 vpos = mul(transform->out, vec3{vertex.position.x, vertex.position.y, 1.0});
        float w = box.x/2;
        float h = box.y/2;
        if (vpos.x >= pos.x - w &&
            vpos.x <= pos.x + w &&
            vpos.y >= pos.y - h &&
            vpos.y <= pos.y + h) {
            return true;
        }
    }
    return false;
}
