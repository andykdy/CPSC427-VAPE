//
// Created by Cody on 11/9/2019.
//

#ifndef VAPE_LEVEL1_HPP
#define VAPE_LEVEL1_HPP

#include "../Entities/Bosses/Boss1.hpp"
#include "Waves.hpp"
#include "Level2.hpp"

namespace Levels {
    const Timeline level1Timeline = {
            {1500,  TSpaced3},

            {4000,  LSpaced3},

            {8000,  RSpaced3},

            {11000, TL3},
            {12500, TR3},
            {14000, TM3},
            {16000, TSpaced4},

            {20000, LM2GenericShooter},
            {20500, RM2GenericShooter},
            {21000, TMPickupSingle},

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
            {46500, TSpaced4GenericShooter},
            // {46500, TSpaced4Fast},
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
            {58100, TSpaced4GenericShooter},
            {58101, RSpaced4},
            {58102, TRPickupSingle},


            {62000, LM3Fast},
            {62500, RM3Fast},
            {63000, TL3Fast},
            {63001, TR3Fast},

            {66000, LM2GenericShooter},
            {66500, TSpaced4},
            {67001, TR3Fast},

            {69000, LM3},
            {69001, RM3},
            {69002, TSpaced4},

            {71000, LM2GenericShooter},
            {71001, RM2GenericShooter},

            {74000, TM3},
            {74500, TL3Fast},
            {74501, TR3Fast},

            {77000, TSpaced4Fast},
            {77100, TSpaced4GenericShooter},
            {77101, LSpaced4},
            {77200, TLPickupSingle}
    };

    const Level level1 = Level(
            1,
            //{},
            level1Timeline,
            spawnBoss<Boss1>,
            //10000,
            90000,
            textures_path("space_bg.png"),
            audio_path("music_level1.wav"),
            audio_path("music_boss1.wav"),
            "Dialogues/Boss1Dialogue.png",
            &level2
    );
}

#endif //VAPE_LEVEL1_HPP
