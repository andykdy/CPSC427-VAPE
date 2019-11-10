//
// Created by Cody on 11/9/2019.
//

#ifndef VAPE_LEVEL2_HPP
#define VAPE_LEVEL2_HPP

#include "../Entities/Bosses/Boss2.hpp"
#include "Waves.hpp"

namespace Levels {

    const Timeline level2Timeline = {

    };

    const Level level2 = Level(
            level2Timeline,
            spawnBoss<Boss2>,
            10000,
            textures_path("space_bg2.png"), // TODO change
            audio_path("music_level2.wav"),
            audio_path("music_boss2.wav"),
            "Boss2Dialogue.png"
    );
}


#endif //VAPE_LEVEL2_HPP
