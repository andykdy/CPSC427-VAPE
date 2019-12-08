//
// Created by Cody on 11/26/2019.
//

#ifndef VAPE_LEVEL3_HPP
#define VAPE_LEVEL3_HPP

#include "../Entities/Bosses/Boss3.hpp"
#include "Waves.hpp"

namespace Levels {
    const Timeline level3Timeline = {
		{1500, TM3},
		{3000, RM3},
		{4500, LM3},

		{6000, TSpaced2GenericShooter},
		{7000, TLDiag3},
		{8500, TRExplosiveSingle},

		{10000, TSpaced4},
		{11000, TSpaced2GenericShooter},

		{12000, TLExplosiveSingle},

		{14000, TLTargetSingle},
		{14001, TRTargetSingle},
		{15000, TM3},

		{ 18000, LM3Fast},
		{ 18001, RM3Fast},
		{ 18002, TSpaced4Fast },

		{21000, TExplosiveDouble},

		{22500, TMTargetSingle},
		{23500, LM3},
		{23501, RM3},

		{25000, TL3Fast},
		{25500, TR3Fast},

		{26500, TLTargetSingle},
		{27500, TRTargetSingle},

		{28000, TLDiag3},
		{28001, TRDiag3},

		{30000, LM2GenericShooter},
		{32000, RM2GenericShooter},
		{33000, TMExplosiveSingle},

		{34000, TSpaced4},
		{35000, TRDiag3},
		{35500, LM3},
		{36000, TLDiag3},
		{36500, RM3},

		{38000, TExplosiveTriple},

		{40000, TMTargetSingle},
		{41000, TLDiag3},
		{41001, TRDiag3},
		{42000, TSpaced4},

		{44000, TSpaced4Fast},
		{44500, TM3},
		{45000, TL3},
		{45001, TR3},
		{45002, LSpaced4},
		{45003, RSpaced4}
    };

    const Level level3 = Level(
            3,
            level3Timeline,
            spawnBoss<Boss3>,
			54500,
            textures_path("space_bg3.png"),
            audio_path("music_level3.wav"),
            audio_path("music_boss3.wav"),
            "Dialogues/Boss3Dialogue.png"
    );
}
#endif //VAPE_LEVEL3_HPP
