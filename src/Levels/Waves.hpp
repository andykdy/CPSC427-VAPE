//
// Created by Cody on 11/9/2019.
//

#ifndef VAPE_WAVES_HPP
#define VAPE_WAVES_HPP

#include "Level.hpp"

namespace Levels {

    /*
     * Turtle Waves
     */
    const float Tspd = 180.f;
    const vec2 TD = {0.f, Tspd};
    const vec2 TD2 = {0.f, 2 * Tspd};
    const vec2 TU = {0.f, -Tspd};
    const vec2 TL = {-Tspd, 0.f};
    const vec2 TL2 = {-2 * Tspd, 0.f};
    const vec2 TR = {Tspd, 0.f};
    const vec2 TR2 = {2 * Tspd, 0.f};

    const vec2 TDR = {Tspd, Tspd};
    const vec2 TDL = {-Tspd, Tspd};

// TODO pull these from elsewhere (screen w/h
    const size_t SW = 800;
    const size_t SH = 1000;



    /*
     *
     * Turtle-only Waves
     *
     */
    const Wave TL3 = {genSpawn(spawn < Turtle > , {100, -100}, TD, 0.f),
                      genSpawn(spawn < Turtle > , {200, -150}, TD, 0.f),
                      genSpawn(spawn < Turtle > , {300, -100}, TD, 0.f)
    };

    const Wave TL3Fast = {genSpawn(spawn < Turtle > , {100, -100}, TD2, 0.f),
                          genSpawn(spawn < Turtle > , {200, -150}, TD2, 0.f),
                          genSpawn(spawn < Turtle > , {300, -100}, TD2, 0.f)
    };

    const Wave TM3 = {genSpawn(spawn < Turtle > , {300, -100}, TD, 0.f),
                      genSpawn(spawn < Turtle > , {400, -150}, TD, 0.f),
                      genSpawn(spawn < Turtle > , {500, -100}, TD, 0.f)
    };
    const Wave TR3 = {genSpawn(spawn < Turtle > , {500, -100}, TD, 0.f),
                      genSpawn(spawn < Turtle > , {600, -150}, TD, 0.f),
                      genSpawn(spawn < Turtle > , {700, -100}, TD, 0.f)
    };
    const Wave TR3Fast = {genSpawn(spawn < Turtle > , {500, -100}, TD2, 0.f),
                          genSpawn(spawn < Turtle > , {600, -150}, TD2, 0.f),
                          genSpawn(spawn < Turtle > , {700, -100}, TD2, 0.f)
    };

    const Wave TSpaced4 = {
            genSpawn(spawn < Turtle > , {150, -100}, TD, 0.f),
            genSpawn(spawn < Turtle > , {300, -100}, TD, 0.f),
            genSpawn(spawn < Turtle > , {450, -100}, TD, 0.f),
            genSpawn(spawn < Turtle > , {600, -100}, TD, 0.f),
    };

    const Wave TSpaced4Fast = {
            genSpawn(spawn < Turtle > , {150, -100}, TD2, 0.f),
            genSpawn(spawn < Turtle > , {300, -100}, TD2, 0.f),
            genSpawn(spawn < Turtle > , {450, -100}, TD2, 0.f),
            genSpawn(spawn < Turtle > , {600, -100}, TD2, 0.f),
    };


    const Wave TSpaced3 = {
            genSpawn(spawn < Turtle > , {250, -100}, TD, 0.f),
            genSpawn(spawn < Turtle > , {400, -100}, TD, 0.f),
            genSpawn(spawn < Turtle > , {550, -100}, TD, 0.f),
    };


    const Wave LM3 = {genSpawn(spawn < Turtle > , {-100, 200}, TR, 0.f),
                      genSpawn(spawn < Turtle > , {-150, 400}, TR, 0.f),
                      genSpawn(spawn < Turtle > , {-200, 600}, TR, 0.f)
    };

    const Wave LM3Low = {genSpawn(spawn < Turtle > , {-100, 300}, TR, 0.f),
                      genSpawn(spawn < Turtle > , {-150, 500}, TR, 0.f),
                      genSpawn(spawn < Turtle > , {-200, 700}, TR, 0.f)
    };

    const Wave LM3Fast = {genSpawn(spawn < Turtle > , {-100, 200}, TR2, 0.f),
                          genSpawn(spawn < Turtle > , {-150, 400}, TR2, 0.f),
                          genSpawn(spawn < Turtle > , {-200, 600}, TR2, 0.f)
    };
    const Wave LM3FastLow = {genSpawn(spawn < Turtle > , {-100, 300}, TR2, 0.f),
                          genSpawn(spawn < Turtle > , {-150, 500}, TR2, 0.f),
                          genSpawn(spawn < Turtle > , {-200, 700}, TR2, 0.f)
    };

    const Wave LSpaced4 = {genSpawn(spawn < Turtle > , {-100, 150}, TR, 0.f),
                           genSpawn(spawn < Turtle > , {-100, 350}, TR, 0.f),
                           genSpawn(spawn < Turtle > , {-100, 550}, TR, 0.f),
                           genSpawn(spawn < Turtle > , {-100, 750}, TR, 0.f)
    };

    const Wave LSpaced3 = {
            genSpawn(spawn < Turtle > , {-100, 200}, TR, 0.f),
            genSpawn(spawn < Turtle > , {-100, 500}, TR, 0.f),
            genSpawn(spawn < Turtle > , {-100, 800}, TR, 0.f),
    };

    const Wave TLDiag3 = {
            genSpawn(spawn < Turtle > , {0, -100}, TDR, 0.f),
            genSpawn(spawn < Turtle > , {-100, -100}, TDR, 0.f),
            genSpawn(spawn < Turtle > , {-200, -0}, TDR, 0.f)
    };

    const Wave RM3 = {genSpawn(spawn < Turtle > , {SW + 100, 300}, TL, 0.f),
                      genSpawn(spawn < Turtle > , {SW + 150, 500}, TL, 0.f),
                      genSpawn(spawn < Turtle > , {SW + 300, 700}, TL, 0.f)
    };

    const Wave RM3Fast = {genSpawn(spawn < Turtle > , {SW + 100, 300}, TL2, 0.f),
                          genSpawn(spawn < Turtle > , {SW + 150, 500}, TL2, 0.f),
                          genSpawn(spawn < Turtle > , {SW + 300, 700}, TL2, 0.f)
    };

    const Wave RSpaced4 = {genSpawn(spawn < Turtle > , {SW + 100, 250}, TL, 0.f),
                           genSpawn(spawn < Turtle > , {SW + 100, 450}, TL, 0.f),
                           genSpawn(spawn < Turtle > , {SW + 100, 650}, TL, 0.f),
                           genSpawn(spawn < Turtle > , {SW + 100, 850}, TL, 0.f)
    };

    const Wave RSpaced3 = {
            genSpawn(spawn < Turtle > , {SW + 100, 200}, TL, 0.f),
            genSpawn(spawn < Turtle > , {SW + 100, 500}, TL, 0.f),
            genSpawn(spawn < Turtle > , {SW + 100, 800}, TL, 0.f),
    };

    const Wave TRDiag3 = {
            genSpawn(spawn < Turtle > , {SW + 0, -100}, TDL, 0.f),
            genSpawn(spawn < Turtle > , {SW + 100, -100}, TDL, 0.f),
            genSpawn(spawn < Turtle > , {SW + 200, -0}, TDL, 0.f)
    };




    /*
     *
     * Generic Shooter Waves
     *
     *
     */
    const Wave TSpaced3GenericShooter = {
            genSpawn(spawn < EnemyGenericShooter > , {250, -100}, TD, 0.f),
            genSpawn(spawn < EnemyGenericShooter > , {400, -200}, TD, 0.f),
            genSpawn(spawn < EnemyGenericShooter > , {550, -100}, TD, 0.f),
    };

    const Wave TSpaced4GenericShooter = {
            genSpawn(spawn < EnemyGenericShooter > , {150, -100}, TD, 0.f),
            genSpawn(spawn < EnemyGenericShooter > , {300, -100}, TD, 0.f),
            genSpawn(spawn < EnemyGenericShooter > , {450, -100}, TD, 0.f),
            genSpawn(spawn < EnemyGenericShooter > , {600, -100}, TD, 0.f),
    };



    const Wave LM2GenericShooter = {genSpawn(spawn < EnemyGenericShooter > , {-100, 200}, TR, 0.f),
                                    genSpawn(spawn < EnemyGenericShooter > , {-150, 400}, TR, 0.f),
    };
    const Wave RM2GenericShooter = {genSpawn(spawn < EnemyGenericShooter > , {SW + 100, 300}, TL, 0.f),
                                    genSpawn(spawn < EnemyGenericShooter > , {SW + 150, 500}, TL, 0.f),
    };





    /*
     *
     * Targetted Shooter Waves
     *
     */

    const Wave TLTargetSingle = {genSpawn(spawn < EnemyTargettedShooter > , {100, -100}, TD, 0.f)};
    const Wave TRTargetSingle = {genSpawn(spawn <EnemyTargettedShooter>, {SW-100, -100}, TD, 0.f)};
    const Wave TMTargetSingle = {genSpawn(spawn <EnemyTargettedShooter>, {SW/2, -100}, TD, 0.f)};

    /*
     *
     * Explosive Payload Waves
     *
     */
    const Wave TLExplosiveSingle = {genSpawn(spawn < EnemyExplosivePlayload > , {100, -100}, TD, 0.f)};

    /*
     *
     * Speedster Waves
     *
     */
    const Wave TMSpeedsterSingle = {genSpawn(spawn <EnemySpeedster>, {SW/2, -100}, TD, 0.f)};

    const Wave LM2Speedster = {genSpawn(spawn < EnemySpeedster > , {-100, 200}, TR, 0.f),
                              genSpawn(spawn < EnemySpeedster > , {-150, 400}, TR, 0.f),};

    const Wave LRM2Speedster = {genSpawn(spawn < EnemySpeedster > , {-100, 200}, TR, 0.f),
                                genSpawn(spawn < EnemySpeedster > , {-150, 400}, TL, 0.f),};
}
#endif //VAPE_WAVES_HPP
