//
// Created by Cody on 10/20/2019.
//

#ifndef VAPE_LEVEL_HPP
#define VAPE_LEVEL_HPP

#include <map>
#include <Engine/ECS/ECS.hpp>
#include <Entities/Enemy.hpp>
#include <Entities/turtle.hpp>

// TODO change turtle to enemy superclass?

namespace Levels {
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

    inline spawnStruct genSpawn(Spawner fn, vec2 pos, vec2 vel, float dir) {
        spawnStruct s{};
        s.fn = fn;
        s. pos = pos;
        s.dir = dir;
        s.vel = vel;
        return s;
    }

    const float Tspd = 180.f;
    const vec2 TD = {0.f, Tspd};
    const vec2 TD2 = {0.f, 2*Tspd};
    const vec2 TU = {0.f, -Tspd};
    const vec2 TL = {-Tspd, 0.f};
    const vec2 TL2 = {-2*Tspd, 0.f};
    const vec2 TR = {Tspd, 0.f};
    const vec2 TR2 = {2*Tspd, 0.f};

    const vec2 TDR = {Tspd, Tspd};
    const vec2 TDL = {-Tspd, Tspd};

    // TODO pull these from elsewhere (screen w/h
    const size_t SW = 800;
    const size_t SH = 1000;



    const Wave TL3 = { genSpawn(spawn<Turtle>, {100, -100}, TD, 0.f),
                genSpawn(spawn<Turtle>, {200, -150}, TD, 0.f),
                genSpawn(spawn<Turtle>, {300, -100}, TD, 0.f)
    };

    const Wave TL3Fast = { genSpawn(spawn<Turtle>, {100, -100}, TD2, 0.f),
                           genSpawn(spawn<Turtle>, {200, -150}, TD2, 0.f),
                           genSpawn(spawn<Turtle>, {300, -100}, TD2, 0.f)
    };

    const Wave TM3 = { genSpawn(spawn<Turtle>, {300, -100}, TD, 0.f),
                       genSpawn(spawn<Turtle>, {400, -150}, TD, 0.f),
                       genSpawn(spawn<Turtle>, {500, -100}, TD, 0.f)
    };
    const Wave TR3 = { genSpawn(spawn<Turtle>, {500, -100}, TD, 0.f),
                       genSpawn(spawn<Turtle>, {600, -150}, TD, 0.f),
                       genSpawn(spawn<Turtle>, {700, -100}, TD, 0.f)
    };
    const Wave TR3Fast = { genSpawn(spawn<Turtle>, {500, -100}, TD2, 0.f),
                           genSpawn(spawn<Turtle>, {600, -150}, TD2, 0.f),
                           genSpawn(spawn<Turtle>, {700, -100}, TD2, 0.f)
    };

    const Wave TSpaced4 = {
        genSpawn(spawn<Turtle>, {150, -100}, TD, 0.f),
        genSpawn(spawn<Turtle>, {300, -100}, TD, 0.f),
        genSpawn(spawn<Turtle>, {450, -100}, TD, 0.f),
        genSpawn(spawn<Turtle>, {600, -100}, TD, 0.f),
    };

    const Wave TSpaced4Fast = {
            genSpawn(spawn<Turtle>, {150, -100}, TD2, 0.f),
            genSpawn(spawn<Turtle>, {300, -100}, TD2, 0.f),
            genSpawn(spawn<Turtle>, {450, -100}, TD2, 0.f),
            genSpawn(spawn<Turtle>, {600, -100}, TD2, 0.f),
    };


    const Wave TSpaced3 = {
            genSpawn(spawn<Turtle>, {250, -100}, TD, 0.f),
            genSpawn(spawn<Turtle>, {400, -100}, TD, 0.f),
            genSpawn(spawn<Turtle>, {550, -100}, TD, 0.f),
    };


    const Wave LM3 = { genSpawn(spawn<Turtle>, {-100, 200}, TR, 0.f),
                       genSpawn(spawn<Turtle>, {-150, 400}, TR, 0.f),
                       genSpawn(spawn<Turtle>, {-200, 600}, TR, 0.f)
    };

    const Wave LM3Fast = { genSpawn(spawn<Turtle>, {-100, 200}, TR2, 0.f),
                           genSpawn(spawn<Turtle>, {-150, 400}, TR2, 0.f),
                           genSpawn(spawn<Turtle>, {-200, 600}, TR2, 0.f)
    };

    const Wave LSpaced4 = { genSpawn(spawn<Turtle>, {-100, 150}, TR, 0.f),
                            genSpawn(spawn<Turtle>, {-100, 350}, TR, 0.f),
                            genSpawn(spawn<Turtle>, {-100, 550}, TR, 0.f),
                            genSpawn(spawn<Turtle>, {-100, 750}, TR, 0.f)
    };

    const Wave LSpaced3 = {
            genSpawn(spawn<Turtle>, {-100, 200}, TR, 0.f),
            genSpawn(spawn<Turtle>, {-100, 500}, TR, 0.f),
            genSpawn(spawn<Turtle>, {-100, 800}, TR, 0.f),
    };

    const Wave TLDiag3 = {
            genSpawn(spawn<Turtle>, {0, -100}, TDR, 0.f),
            genSpawn(spawn<Turtle>, {-100, -100}, TDR, 0.f),
            genSpawn(spawn<Turtle>, {-200, -0}, TDR, 0.f)
    };

    const Wave RM3 = { genSpawn(spawn<Turtle>, {SW+100, 300}, TL, 0.f),
                       genSpawn(spawn<Turtle>, {SW+150, 500}, TL, 0.f),
                       genSpawn(spawn<Turtle>, {SW+300, 700}, TL, 0.f)
    };

    const Wave RM3Fast = { genSpawn(spawn<Turtle>, {SW+100, 300}, TL2, 0.f),
                       genSpawn(spawn<Turtle>, {SW+150, 500}, TL2, 0.f),
                       genSpawn(spawn<Turtle>, {SW+300, 700}, TL2, 0.f)
    };

    const Wave RSpaced4 = { genSpawn(spawn<Turtle>, {SW+100, 250}, TL, 0.f),
                            genSpawn(spawn<Turtle>, {SW+100, 450}, TL, 0.f),
                            genSpawn(spawn<Turtle>, {SW+100, 650}, TL, 0.f),
                            genSpawn(spawn<Turtle>, {SW+100, 850}, TL, 0.f)
    };

    const Wave RSpaced3 = {
            genSpawn(spawn<Turtle>, {SW+100, 200}, TL, 0.f),
            genSpawn(spawn<Turtle>, {SW+100, 500}, TL, 0.f),
            genSpawn(spawn<Turtle>, {SW+100, 800}, TL, 0.f),
    };

    const Wave TRDiag3 = {
            genSpawn(spawn<Turtle>, {SW+0, -100}, TDL, 0.f),
            genSpawn(spawn<Turtle>, {SW+100, -100}, TDL, 0.f),
            genSpawn(spawn<Turtle>, {SW+200, -0}, TDL, 0.f)
    };


    // TODO add out of bounds checks for these

    const Timeline level1 = {
            {1500,  TSpaced3},

            {4000,  LSpaced3},

            {8000,  RSpaced3},

            {11000, TL3},
            {12500, TR3},
            {14000, TM3},
            {16000, TSpaced4},

            {20000, LM3},
            {20500, RM3},

            {25000, TL3},
            {25001, TR3},
            {25500, TM3},

            {30000, TSpaced4},
            {30001, LSpaced4},
            {30002, RSpaced4},

            {35000, TSpaced4Fast},

            {36500, TLDiag3},
            {36500, TRDiag3},

            {38000, TSpaced4},
            {39000, TRDiag3},
            {39500, LM3},
            {40000, TLDiag3},
            {40500, RM3},

            {42000, TSpaced4Fast},
            {42500, TM3},
            {43000, TL3},
            {43001, TR3},
            {43002, LSpaced4},
            {43003, RSpaced4},

            {45000, LM3Fast},
            {45500, RM3Fast},
            {46000, TL3Fast},
            {46001, TR3Fast},
            {46500, TSpaced4Fast},
            {48000, TRDiag3},
            {48001, TLDiag3},
            {49000, TL3},
            {49001, TR3},
            {49500, LSpaced4},
            {50000, RSpaced4},

            {53000, TM3},
            {53500, TL3Fast},
            {53501, TR3Fast},

            {55000, LSpaced4},
            {56000, RSpaced4},

            {58000, TSpaced4Fast},

    };

}

#endif //VAPE_LEVEL_HPP
