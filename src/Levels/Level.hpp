//
// Created by Cody on 10/20/2019.
//

#ifndef VAPE_LEVEL_HPP
#define VAPE_LEVEL_HPP

#include <map>
#include <Engine/ECS/ECS.hpp>
#include <Entities/Bosses/Boss.hpp>
#include <Entities/Enemies/Enemy.hpp>
#include <Entities/Enemies/turtle.hpp>
#include "Entities/Enemies/EnemyGenericShooter.hpp"
#include "Entities/Enemies/EnemyTargettedShooter.hpp"
#include "Entities/Enemies/EnemyExplosivePayload.hpp"
#include "Entities/Enemies/EnemySpeedster.hpp"

namespace Levels {
    typedef Boss* BossSpawner(ECS::EntityManager *e);
    typedef Enemy* Spawner(ECS::EntityManager *e, vec2 pos, vec2 vel, float dir);

    struct spawnStruct {
        Spawner* fn;
        vec2 pos;
        vec2 vel;
        float dir;
    };

    using Wave = std::vector<spawnStruct>;
    using Timeline = std::map<int, Wave>; // NOTE - YOU CANNOT HAVE WAVES SPAWN AT THE EXACT SAME MS (ITS A MAP)

    template <typename T> Enemy* spawn(ECS::EntityManager *e, vec2 pos, vec2 vel, float dir) {
        Enemy* t = &e->addEntity<T>();
        t->init();
        t->set_position(pos);
        t->set_velocity(vel);
        return t;
        // TODO set direction?
    }

    template <typename T> Boss* spawnBoss(ECS::EntityManager *e) {
        Boss* b = &e->addEntity<T>();
        return b;
    }

    inline spawnStruct genSpawn(Spawner fn, vec2 pos, vec2 vel, float dir) {
        spawnStruct s{};
        s.fn = fn;
        s. pos = pos;
        s.dir = dir;
        s.vel = vel;
        return s;
    }

    struct Level {
        unsigned int id;
        Timeline timeline;
        BossSpawner* spawnBoss;
        size_t bossTime;

        const char* backgroundTexture;
        const char* backgroundMusic;
        const char* bossMusic;
        const char* bossDialogue;

        const Level* nextLevel = nullptr;

        Level(unsigned int id, const Timeline t, BossSpawner bs, size_t time, const char* bt, const char* bgm, const char* bm, const char* bd, const Level *next = nullptr) :
                id(id), timeline(t), spawnBoss(bs), bossTime(time), backgroundTexture(bt), backgroundMusic(bgm), bossMusic(bm), bossDialogue(bd), nextLevel(next)  {}
    };

}

#endif //VAPE_LEVEL_HPP
