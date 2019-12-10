//
// Created by Cody on 11/29/2019.
//

#ifndef VAPE_PHYSFSHELPERS_HPP
#define VAPE_PHYSFSHELPERS_HPP

#include <Engine/GameEngine.hpp>

class RWFile {
private:
    uint8_t* m_data;
    int64_t m_size;
    SDL_RWops * m_rw;
public:
    RWFile();

    bool init(const char* path);
    void destroy();

    uint8_t *getM_data() const;

    int64_t getM_size() const;

    void setM_rw(SDL_RWops *m_rw);
};

Mix_Music* Load_Music(RWFile);

Mix_Chunk* Load_Wav(RWFile);

#endif //VAPE_PHYSFSHELPERS_HPP
