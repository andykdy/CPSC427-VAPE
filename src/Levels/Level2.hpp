//
// Created by Cody on 11/9/2019.
//

#ifndef VAPE_LEVEL2_HPP
#define VAPE_LEVEL2_HPP

#include "../Entities/Bosses/Boss2.hpp"
#include "Waves.hpp"

namespace Levels {

    const Timeline level2Timeline = {
            { 1500, TSpaced3GenericShooter },

            { 4000, TLTargetSingle },

            { 8000, TSpaced4Fast },
            { 9000, TSpaced3 },

            { 12000, RM3 },
            { 12000, LM2GenericShooter },

            { 15000, TLTargetSingle},
            { 16000, TRTargetSingle},

            { 18000, LM3Fast},
            { 18001, RM3Fast},
            { 18002, TSpaced4Fast },

            {20000, TSpaced4Fast},
            {20500, TM3},
            {21002, LM2GenericShooter},
            {21003, RM2GenericShooter},

            {22500, TSpaced4},
            {22501, LSpaced4},
            {22502, RSpaced4},

            {23500, TLTargetSingle},
            {24000, TRTargetSingle},
            {24500, TMTargetSingle},

            {28000, LM3Fast},
            {28500, RM3Fast},
            {29000, TL3Fast},
            {29001, TR3Fast},
            {29500, TSpaced4Fast},
            {31000, TRDiag3},
            {31001, TLDiag3},
            {32000, TL3},
            {32001, TR3},
            {32002, TSpaced4GenericShooter},
            {32500, LSpaced4},
            {33000, RSpaced4},

            {35000, LM2GenericShooter},
            {35001, RM2GenericShooter},

    };

    const Level level2 = Level(
            {},// level2Timeline,
            spawnBoss<Boss2>,
            10000,//47000,
            textures_path("space_bg2.png"),
            audio_path("music_level2.wav"),
            audio_path("music_boss2.wav"),
            "Dialogues/Boss2Dialogue.png"
    );
}


#endif //VAPE_LEVEL2_HPP
