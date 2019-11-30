//
// Created by Cody on 11/29/2019.
//

#ifndef VAPE_PHYSFSHELPERS_HPP
#define VAPE_PHYSFSHELPERS_HPP

#include <SDL_mixer.h>

Mix_Music* Load_Music(const char* path);

Mix_Chunk* Load_Wav(const char* path);

#endif //VAPE_PHYSFSHELPERS_HPP
