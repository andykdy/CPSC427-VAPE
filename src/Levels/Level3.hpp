//
// Created by Cody on 11/26/2019.
//

#ifndef VAPE_LEVEL3_HPP
#define VAPE_LEVEL3_HPP

#include "../Entities/Bosses/Boss3.hpp"
#include "Waves.hpp"

namespace Levels {
    const Timeline level3Timeline = {
			//{ 1500, TLExplosiveSingle },
			{ 1500, TMPickupSingle },

            { 4500, TLExplosiveSingle }
    };

    const Level level3 = Level(
            3,
            level3Timeline,
            spawnBoss<Boss3>,
            10000,
            textures_path("space_bg3.png"),
            audio_path("music_level3.wav"),
            audio_path("music_boss3.wav"),
            "Dialogues/Boss3Dialogue.png"
    );
}
#endif //VAPE_LEVEL3_HPP
