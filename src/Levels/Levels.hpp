//
// Created by Cody on 11/9/2019.
//

#ifndef VAPE_LEVELS_HPP
#define VAPE_LEVELS_HPP

// Standard levels header, import this elsewhere for access to all level stuff.

#include "Level1.hpp"
#include "Level2.hpp"

namespace Levels {
    // maps specific levels to a levelId, for saving/loading purposes
    const std::map<unsigned int, const Level*> level_map = {
            {level1.id, &level1},
            {level2.id, &level2},
    };
}

#endif //VAPE_LEVELS_HPP
