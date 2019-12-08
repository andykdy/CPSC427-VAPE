//
// Created by Cody on 11/29/2019.
//

#include "PhysFSHelpers.hpp"
#include <physfs.h>
#include <iostream>

bool RWFile::init(const char *path) {
    if (!PHYSFS_exists(path))
    {
        std::cout << "Unable to find " << path << std::endl;
        return false; //file doesn't exist
    }

    PHYSFS_file* myfile = PHYSFS_openRead(path);

    // Get the lenght of the file
    m_size = PHYSFS_fileLength(myfile);

    // Get the file data.
    m_data = new uint8_t[m_size];

    auto length_read = PHYSFS_readBytes(myfile, m_data, static_cast<PHYSFS_uint64>(m_size));

    if (length_read != m_size)
    {
        std::cout << PHYSFS_getLastErrorCode() << std::endl;
        delete [] m_data;
        m_data = nullptr;
        return false;
    }

    PHYSFS_close(myfile);

    return true;
}

void RWFile::destroy() {
    if (m_rw != nullptr)
        SDL_FreeRW(m_rw);
    delete [] m_data;
    m_data = nullptr;
}

RWFile::RWFile() {
    m_rw = nullptr;
    m_data = nullptr;
    m_size = 0;
}

uint8_t *RWFile::getM_data() const {
    return m_data;
}

int64_t RWFile::getM_size() const {
    return m_size;
}

void RWFile::setM_rw(SDL_RWops *m_rw) {
    RWFile::m_rw = m_rw;
}

Mix_Music *Load_Music(RWFile file) {
    SDL_RWops* rw = SDL_RWFromMem(file.getM_data(), file.getM_size());
    file.setM_rw(rw);
    return Mix_LoadMUS_RW(rw, 0);
}

Mix_Chunk *Load_Wav(RWFile file) {
    SDL_RWops* rw = SDL_RWFromMem(file.getM_data(), file.getM_size());
    file.setM_rw(rw);
    return Mix_LoadWAV_RW(rw, 0);
}
