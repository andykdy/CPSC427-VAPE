//
// Created by Cody on 11/9/2019.
//

#ifndef VAPE_LEVEL2_HPP
#define VAPE_LEVEL2_HPP

#include "../Entities/Bosses/Boss2.hpp"
#include "Waves.hpp"
#include "Level3.hpp"

namespace Levels {

    const Timeline level2Timeline = {
            {1500,  TSpaced3GenericShooter},

            {4000,  TLTargetSingle},

            {6000, TMSpeedsterSingle},

            {8000,  TSpaced4Fast},
            {9000,  TSpaced3},

            {12000, RM3},
            {12000, LM2GenericShooter},

            {15000, TLTargetSingle},
            {16000, TRTargetSingle},

            {18000, LM3Fast},
            {18001, RM3Fast},
            {18002, TSpaced4Fast},
            {19000, TLPickupSingle},

            {20000, TSpaced4Fast},
            {20500, TM3},
            {21002, LM2GenericShooter},
            {21003, RM2GenericShooter},

            {22500, TSpaced4},
            {22501, LM2Speedster},
            {22502, RM2Speedster},

            {23500, TLTargetSingle},
            {24000, TRTargetSingle},
            {24500, TMTargetSingle},

            {28000, LM3Fast},
            {28500, RM3Fast},
            {29000, TL3Fast},
            {29001, TR3Fast},
            {29500, TSpaced4Fast},
            {30000, TRPickupSingle},
            {31000, TRDiag3},
            {31001, TLDiag3},
            {32000, TL3},
            {32001, TR3},
            {32002, TSpaced4GenericShooter},
            {32500, LM3Speedster},
            {33000, RM3Speedster},

            {35000, LM2GenericShooter},
            {35001, RM2GenericShooter},

            {37000, TL3Fast},
            {37100, TR3Fast},
            {38500, TLTargetSingle},
            {38501, TRTargetSingle},

            {40000, LM3Fast},
            {40500, RM3Fast},
            {41000, TL3Fast},
            {41001, TR3Fast},
            {41500, TSpaced4GenericShooter},
            {43000, TRDiag3},
            {43001, TLDiag3},
            {43002, TMPickupSingle},
            {44000, TL3},
            {44001, TR3},
            {44500, LSpaced4},
            {45000, RSpaced4},

            {49000, LM2GenericShooter},
            {50000, RM2GenericShooter},
            {51000, TMTargetSingle},

            {52000, LM3Speedster},

            {53000, TLDiag3},
            {53001, TRDiag3},
            {54000, TSpaced4GenericShooter},

            {55000, TRDiag3},
            {55001, TLDiag3},
            {56000, TSpaced4Fast},

            {57000, TSPaced4Speedster},
            {58000, TRTargetSingle},
            {58500, LM3},
            {59000, TRTargetSingle},
            {59001, TRPickupSingle},
            {59500, RM3},

            {60000, TSpaced4Fast},

            {63000, LM2GenericShooter},
            {63001, TSpaced4GenericShooter},
            {63002, TLDiag3},

            {66000, LSpaced4},
            {66500, LM2GenericShooter},
            {68000, RSpaced4},
            {68500, RM2GenericShooter},

            {70000, TLTargetSingle},
            {70500, TRTargetSingle},
            {71000, TMTargetSingle},

            {72500, TSPaced4Speedster},
            {73000, RM3Speedster},
            {73001, TRPickupSingle},
            {73500, TSpaced4GenericShooter},

            {76000, RM2GenericShooter},
            {76001, TSpaced4GenericShooter},
            {76002, TRDiag3},

            {78000, LM3Fast},
            {78500, RM3Fast},
            {79000, TL3Fast},
            {79001, TR3Fast},
            {79500, TSpaced4GenericShooter},
            {81000, TRDiag3},
            {81001, TLDiag3},
            {82000, TL3},
            {82001, TR3},
            {82500, LSpaced4},
            {83000, RSpaced4},

            {85000, LM2GenericShooter},
            {86000, RM2GenericShooter},
            {87000, TSPaced4Speedster},
            {87100, TMPickupSingle},

            {88000, LM3Fast},
            {88500, RM3Fast},
            {89000, TL3Fast},
            {89001, TR3Fast},


            {91000, TL3Fast},
            {91001, TR3Fast},
            {91500, TM3Fast},
            {92000, TSpaced4Fast},
            {92500, TL3Fast},
            {92501, TR3Fast},
            {93000, TM3Fast},
            {93500, TSpaced4Fast},
            {95000, TSpaced4GenericShooter},
            {95001, LM3Speedster},
            {95002, RM3Speedster},

            {97000, TL3Fast},
            {97001, TR3Fast},
            {97500, TM3Fast},
            {98000, TSpaced4Fast},
            {98500, TL3Fast},
            {98501, TR3Fast},
            {99000, TM3Fast},
            {99500, TSpaced4Fast},
            {100000, TLTargetSingle},
            {100001, TRTargetSingle},
            {100500, TMTargetSingle},

            {102000, TSPaced4Speedster},
            {103000, TRTargetSingle},
            {103500, LM3},
            {104000, TRTargetSingle},
            {104001, TRPickupSingle},
            {104500, RM3},

    };

    const Level level2 = Level(
            2,
            //{},
            level2Timeline,
            spawnBoss<Boss2>,
            //10000,
            116000,
            textures_path("space_bg2.png"),
            audio_path("music_level2.wav"),
            audio_path("music_boss2.wav"),
            "Dialogues/Boss2Dialogue.png",
            &level3
    );
}


#endif //VAPE_LEVEL2_HPP
