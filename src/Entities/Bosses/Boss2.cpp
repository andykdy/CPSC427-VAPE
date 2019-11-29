//
// Created by Cody on 11/7/2019.
//

#include <Engine/GameEngine.hpp>
#include <Components/SpriteComponent.hpp>
#include <Components/EffectComponent.hpp>
#include <Components/PhysicsComponent.hpp>
#include <Components/MotionComponent.hpp>
#include <Components/TransformComponent.hpp>
#include <Entities/Projectiles and Damaging/Laser/Laser.hpp>
#include <Systems/EnemySpawnerSystem.hpp>
#include <chrono>
#include <random>
#include "Boss2.hpp"


// Same as static in c, local to compilation unit
namespace
{
    const size_t INIT_HEALTH = 500;
    const size_t SPRITE_FRAMES = 4;
    const size_t SPRITE_W = 264;
    const size_t SPRITE_H = 88;
    const size_t DAMAGE_EFFECT_TIME = 100;
    const size_t LASER_DAMAGE = 10;
    const size_t POINTS_VAL = 15000;

    const std::vector<vec2> hardpoints = {
            {44,76},
            {76,72},

            {124,85},
            {140,85},

            {188,72},
            {219,76}
    };

    const vec2 SPRITE_SCALE = {4.5f, 4.5f};
    const vec2 MESH_SCALE = {380.f, 380.f};


    const size_t CHARGE0 = 3000;
    const size_t FIRE0 = 5000;
    const float ROTATION0 = 0.1f;

    const AttackPattern EZ1346 = {
            {true, false, true, true, false, true},
            {0,0,0,0,0,0},
            {0,0,-0.78f,0.78f,0,0},
            {ROTATION0,ROTATION0,ROTATION0,ROTATION0,ROTATION0,ROTATION0},
            {CHARGE0,CHARGE0,CHARGE0,CHARGE0,CHARGE0,CHARGE0},
            {FIRE0,FIRE0,FIRE0,FIRE0,FIRE0,FIRE0},
            8100,
    };


    const AttackPattern EZ25 = {
            {false, true, false, false, true, false},
            {0,0,0,0,0,0},
            {0,0.78f,0,0,-0.78f,0},
            {ROTATION0,ROTATION0,ROTATION0,ROTATION0,ROTATION0,ROTATION0},
            {CHARGE0,CHARGE0,CHARGE0,CHARGE0,CHARGE0,CHARGE0},
            {FIRE0,FIRE0,FIRE0,FIRE0,FIRE0,FIRE0},
            8100
    };


    const AttackPattern EZ123 = {
            {true, true, true, false, false, false},
            {0,0,0,0,0,0},
            {0,0,0,0,0,0},
            {ROTATION0,ROTATION0,ROTATION0,ROTATION0,ROTATION0,ROTATION0},
            {CHARGE0,CHARGE0,CHARGE0,CHARGE0,CHARGE0,CHARGE0},
            {FIRE0,FIRE0,FIRE0,FIRE0,FIRE0,FIRE0},
            8100,

            {Levels::RM3,}
    };

    const AttackPattern EZ456 = {
            {false, false, false, true, true, true},
            {0,0,0,0,0,0},
            {0,0,0,0,0,0},
            {ROTATION0,ROTATION0,ROTATION0,ROTATION0,ROTATION0,ROTATION0},
            {CHARGE0,CHARGE0,CHARGE0,CHARGE0,CHARGE0,CHARGE0},
            {FIRE0,FIRE0,FIRE0,FIRE0,FIRE0,FIRE0},
            8100,

            {Levels::LM3Low,}
    };

    const AttackPattern EZ2345 = {
            {false, true, true, true, true, false},
            {0,0,0,0,0,0},
            {0,0,0,0,0,0},
            {ROTATION0,ROTATION0,ROTATION0,ROTATION0,ROTATION0,ROTATION0},
            {CHARGE0,CHARGE0,CHARGE0,CHARGE0,CHARGE0,CHARGE0},
            {FIRE0,FIRE0,FIRE0,FIRE0,FIRE0,FIRE0},
            8100
    };

    const std::vector<AttackPattern> EasyPatterns = {
            EZ1346,
            EZ25,
            EZ123,
            EZ456,
            EZ2345
    };


    const size_t CHARGE1 = 1500;
    const size_t FIRE1 = 3000;

    const AttackPattern M1346 = {
            {true, false, true, true, false, true},
            {0.78f,0,0,0,0,-0.78f},
            {0,0,-0.78f,0.78f,0,0},
            {ROTATION0,ROTATION0,ROTATION0,ROTATION0,ROTATION0,ROTATION0},
            {CHARGE1,CHARGE1,CHARGE1,CHARGE1,CHARGE1,CHARGE1},
            {FIRE1,FIRE1,FIRE1,FIRE1,FIRE1,FIRE1},
            4700
    };

    const AttackPattern M12345 = {
            {true, true, true, true, true, false},
            {0,0,0,0,0,0},
            {0,0,0,0,0,0},
            {ROTATION0,ROTATION0,ROTATION0,ROTATION0,ROTATION0,ROTATION0},
            {CHARGE1-500,CHARGE1,CHARGE1+500,CHARGE1+1000,CHARGE1+1500,CHARGE0},
            {FIRE1,FIRE1,FIRE1,FIRE1,FIRE1,FIRE1},
            6100,

            {Levels::LM3Low,
             Levels::RM3,}
    };

    const AttackPattern M23456 = {
            {false, true, true, true, true, true},
            {0,0,0,0,0,0},
            {0,0,0,0,0,0},
            {ROTATION0,ROTATION0,ROTATION0,ROTATION0,ROTATION0,ROTATION0},
            {CHARGE0,CHARGE1+1500,CHARGE1+1000,CHARGE1+500,CHARGE1,CHARGE1-500},
            {FIRE1,FIRE1,FIRE1,FIRE1,FIRE1,FIRE1},
            6100,

            {Levels::LM3Low,
             Levels::RM3,}
    };

    const AttackPattern MZ123_456 = {
            {true, true, true, true, true, true},
            {-0.18f,-0.18f,-0.18f,0.18f,0.18f,0.18f},
            {0.78f,0.78f,0.78f,-0.78f,-0.78f,-0.78f},
            {ROTATION0,ROTATION0,ROTATION0,ROTATION0,ROTATION0,ROTATION0},
            {CHARGE1+500,CHARGE1,CHARGE1-500,CHARGE1-500,CHARGE1,CHARGE1+500},
            {FIRE1,FIRE1,FIRE1/2,FIRE1/2,FIRE1,FIRE1},
            4600,

            {Levels::TM3,}
    };

    const std::vector<AttackPattern> MediumPatterns = {
            M1346,
            M12345,
            M23456,
            MZ123_456,
    };


    const size_t CHARGE2 = 750;
    const size_t FIRE2 = 1500;
    const float ROTATION2 = 0.3f;

    const AttackPattern H123456_R = {
            {true, true, true, true, true, true},
            {0,0,0,0,0,0},
            {0,0,0,0,0,0},
            {ROTATION2,ROTATION2,ROTATION2,ROTATION2,ROTATION2,ROTATION2},
            {CHARGE2,CHARGE2+250,CHARGE2+500,CHARGE2+750,CHARGE2+1000, CHARGE2+1250},
            {FIRE2,FIRE2,FIRE2,FIRE2,FIRE2,FIRE2},
            CHARGE2+1250+FIRE2,

            {Levels::RM3Fast,Levels::LM3FastLow}
    };

    const AttackPattern H123456_L = {
            {true, true, true, true, true, true},
            {0,0,0,0,0,0},
            {0,0,0,0,0,0},
            {ROTATION2,ROTATION2,ROTATION2,ROTATION2,ROTATION2,ROTATION2},
            {CHARGE2+1250,CHARGE2+1000,CHARGE2+750,CHARGE2+500,CHARGE2+250, CHARGE2},
            {FIRE2,FIRE2,FIRE2,FIRE2,FIRE2,FIRE2},
            CHARGE2+1250+FIRE2,

            {Levels::RM3Fast,Levels::LM3FastLow}
    };


    const AttackPattern H2345_DM = {
            {false, true, true, true, true, false},
            {0,0.78f,-0.78f,0.78f,-0.78f,0},
            {0,0.58f,-0.48f,0.48f,-0.58f,0},
            {ROTATION2,ROTATION2,ROTATION2,ROTATION2,ROTATION2,ROTATION2},
            {CHARGE2+1250,CHARGE2+1000,CHARGE2+750,CHARGE2+500,CHARGE2+250, CHARGE2},
            {FIRE1,FIRE1,FIRE1,FIRE1,FIRE1,FIRE1},
            CHARGE2+1250+FIRE1,
    };

    const AttackPattern H12345_PNT = {
            {true, true, true, true, true, true},
            {0.78f,0.f,-0.78f,0.78f,0.f,-0.78f},
            {0.48f,0.3f,-0.48f,0.48f,-0.3f,-0.48f},
            {ROTATION2,ROTATION2,ROTATION2,ROTATION2,ROTATION2,ROTATION2},
            {CHARGE1,CHARGE1,CHARGE1,CHARGE1,CHARGE1,CHARGE1},
            {FIRE1,FIRE1,FIRE1,FIRE1,FIRE1,FIRE1},
            CHARGE1+FIRE1,
    };

    const std::vector<AttackPattern> HardPatterns = {
            H123456_R,
            H123456_L,
            H2345_DM,
            H12345_PNT
    };
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
    points = POINTS_VAL;

    // Initialize lasers
    for (vec2 hardpoint : hardpoints) {
        Laser* laser = &GameEngine::getInstance().getEntityManager()->addEntity<Laser>();
        laser->init({0,0}, 0, true, LASER_DAMAGE);
        laser->setState(laserState::off);

        // In projectiles for levelstate to use, but also in m_lasers for us to do laser-specific functions
        projectiles.push_back(laser);
        m_lasers.push_back(laser);
    }

    std::random_device rd;
    m_rand = std::default_random_engine(rd());

    m_easy_patterns = EasyPatterns;
    std::shuffle(m_easy_patterns.begin(), m_easy_patterns.end(), m_rand);
    m_medium_patterns = MediumPatterns;
    std::shuffle(m_medium_patterns.begin(), m_medium_patterns.end(), m_rand);
    m_hard_patterns = HardPatterns;
    std::shuffle(m_hard_patterns.begin(), m_hard_patterns.end(), m_rand);

    m_pattern_cursor = 0;
    m_phase=full;

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


    if (health > INIT_HEALTH * 0.66f) {
        if (m_phase != full) {
            m_phase = full;
            m_pattern_cursor = 0;
        }
    } else if (health > INIT_HEALTH * 0.33) {
        if (m_phase != two_thirds) {
            m_phase = two_thirds;
            m_pattern_cursor = 0;
        }
    } else {
        if (m_phase != one_third) {
            m_phase = one_third;
            m_pattern_cursor = 0;
        }
    }

    m_healthbar->setHealth(health);
    if (m_damage_effect_cooldown > 0)
        m_damage_effect_cooldown -= ms;

    // Update lasers
    vec2 bbox = get_bounding_box();
    float left = motion->position.x - bbox.x/2;
    float top = motion->position.y - bbox.y/2;
    for (int i = 0; i < hardpoints.size(); i++) {
        m_lasers[i]->set_position({hardpoints[i].x*physics->scale.x  + left, hardpoints[i].y*physics->scale.y + top});
    }
    for (auto laser : projectiles) {
        laser->update(ms);
    }

    if (health <= 0){
        for (auto laser : m_lasers) {
            laser->setState(laserState::off);
        }
        return;
    }
    if (m_pattern_timer > 0)
        m_pattern_timer -= ms;
    else {
        if (m_phase == full) {
            choosePattern(m_easy_patterns);
        } else if (m_phase == two_thirds) {
            choosePattern(m_medium_patterns);
        } else {
            choosePattern(m_hard_patterns);
        }
        fireLasers(m_pattern);
    }
}

void Boss2::choosePattern(std::vector<AttackPattern>& patterns) {
    if (m_pattern_cursor < patterns.size()) {
        m_pattern = patterns[m_pattern_cursor];
        ++m_pattern_cursor;
    } else {
        std::shuffle(patterns.begin(), patterns.end(), m_rand);
        m_pattern = patterns[0];
        m_pattern_cursor = 1;
    }
    /*
    bool retry = true;
    while(retry) {
        float randi = rand() % patterns.size();
        if (m_pattern == patterns[randi]) {
            continue;
        } else {
            retry = false;
            m_pattern = patterns[randi];
        }
    }
     */
}

void Boss2::fireLasers(AttackPattern pattern) {
    for (int i = 0; i < 6; i++) {
        Laser* laser = m_lasers[i];

        if (pattern.lasers[i]) {
            laser->fire(pattern.chargeTime[i], pattern.fireTime[i]);
        }
        laser->setRotation(pattern.startRotations[i]);
        laser->setRotationTarget(pattern.targetRotations[i]);
    }

    auto & spawn = GameEngine::getInstance().getSystemManager()->getSystem<EnemySpawnerSystem>();
    for (auto& wave : pattern.waves) {
        spawn.spawnWave(wave);
    }

    m_pattern_timer = pattern.nextPatternDelay;
}

void Boss2::draw(const mat3 &projection) {
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

    for (auto laser : projectiles)
        laser->draw(projection);

    if (GameEngine::getInstance().getM_debug_mode()){
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
    vec2 bbox = vamp.get_bounding_box();
    return checkCollision(vamp.get_position(), {bbox.x * 0.5f, bbox.y * 0.5f});
}

bool Boss2::collidesWith(Player &player) {
    auto* motion = getComponent<MotionComponent>();

    vec2 player_box = player.get_bounding_box();
    vec2 player_pos = player.get_position();

    bool collides = checkCollision(player_pos, {player_box.x * 0.9f, player_box.y * 0.9f});
    if (collides) {
        auto* transform = getComponent<TransformComponent>();
        // Find the larges x and y collisions
        vec2 offset = {0,0};
        for(auto vertex : m_vertices) {
            transform->begin();
            transform->translate(motion->position);
            transform->scale(MESH_SCALE);
            transform->rotate(motion->radians + 1.5708f);
            transform->end();
            vec3 vpos = mul(transform->out, vec3{vertex.position.x, vertex.position.y, 1.0});
            float w = player_box.x/2;
            float h = player_box.y/2;
            if (vpos.x >= player_pos.x - w &&
                vpos.x <= player_pos.x + w &&
                vpos.y >= player_pos.y - h &&
                vpos.y <= player_pos.y + h) {
                // Vertex above player
                if (vpos.y < player_pos.y) {
                    float dif = std::fabs(vpos.y - (player_pos.y - h));
                    if (dif > std::fabs(offset.y))
                        offset.y = dif;
                // Vertex below player
                } else {
                    /*
                    float dif = std::fabs((player_pos.y + h) - vpos.y);
                    if (dif > std::fabs(offset.y))
                        offset.y = -dif;
                    */
                }
                // Vertex left of player
                if (vpos.x < player_pos.x) {
                    float dif = std::fabs(vpos.x - (player_pos.x - w));
                    if (dif > std::fabs(offset.x))
                        offset.x = dif;
                // Vertex right of player
                } else {
                    float dif = std::fabs((player_pos.x + w) - vpos.x);
                    if (dif > std::fabs(offset.x))
                        offset.x = -dif;
                }
            }
        }
        player_pos.x += offset.x;
        player_pos.y += offset.y;
        player.set_position(player_pos);

        float player_r = std::max(player_box.x, player_box.y) * 0.4f;
        float boss_r = std::max(get_bounding_box().x, get_bounding_box().y) * 0.4f;

        float boss_mass = boss_r*1000;
        float player_mass = player_r*100;

        vec2 player_vel = player.get_velocity();
        vec2 boss_vel = {0,0};

        float vbp1 = 2*boss_mass/(player_mass+boss_mass);
        float vbp2 = dot(sub(player_vel, boss_vel), sub(player_pos, motion->position)) / (float)pow(len(sub(player_pos, motion->position)), 2);
        vec2 vb = sub(player_vel, mul(sub(player_pos, motion->position), vbp1*vbp2));

        player.set_velocity(vb);
        player.set_acceleration({0,0});
    }
    return collides;
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

bool AttackPattern::operator==(const AttackPattern &rhs) const {
    bool ret = true;
    for (int i = 0; i < 6; i++) {
        ret = ret && (lasers[i] == rhs.lasers[i]);
    }
    // I think all I care about are same lasers
    return ret;
}

bool AttackPattern::operator!=(const AttackPattern &rhs) const {
    return !(rhs == *this);
}
