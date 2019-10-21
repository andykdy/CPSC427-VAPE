//
// Created by Cody on 10/20/2019.
//

#ifndef VAPE_LEVEL_HPP
#define VAPE_LEVEL_HPP

#include <map>
#include <Engine/ECS/ECS.hpp>
#include <Entities/turtle.hpp>

// TODO change turtle to enemy superclass?

namespace Levels {
    typedef Turtle* Spawner(ECS::EntityManager *e, vec2 pos, float dir);

    struct spawnStruct {
        Spawner* fn;
        vec2 pos;
        float dir;
    };

    using Wave = std::vector<spawnStruct>;
    using Timeline = std::map<int, Wave>;

    template <typename T> Turtle* spawn(ECS::EntityManager *e, vec2 pos, float dir) {
        Turtle* t = &e->addEntity<T>();
        t->init();
        t->set_position(pos);
        return t;
        // TODO set direction?
    }

    inline spawnStruct genSpawn(Spawner fn, vec2 pos, float dir) {
        spawnStruct s{};
        s.fn = fn;
        s. pos = pos;
        s.dir = dir;
        return s;
    }

    const Wave T3 = { genSpawn(spawn<Turtle>, {100, 100}, 0.f),
                genSpawn(spawn<Turtle>, {200, 100}, 0.f),
                genSpawn(spawn<Turtle>, {300, 100}, 0.f)
    };

    const Timeline level1 = {
        {1000, T3},
        {10000, T3}
    };

}

#endif //VAPE_LEVEL_HPP
