//
// Created by Cody on 11/29/2019.
//

#include "PhysFSHelpers.hpp"
#include <physfs.h>
#include <iostream>

Mix_Music* Load_Music(const char* path) {
    if (!PHYSFS_exists(path))
    {
        std::cout << "Unable to find " << path << std::endl;
        return nullptr; //file doesn't exist
    }

    PHYSFS_file* myfile = PHYSFS_openRead(path);

    // Get the lenght of the file
    auto m_size = PHYSFS_fileLength(myfile);

    // Get the file data.
    auto m_data = new uint8_t[m_size];

    auto length_read = PHYSFS_readBytes(myfile, m_data, static_cast<PHYSFS_uint64>(m_size));

    if (length_read != m_size)
    {
        std::cout << PHYSFS_getLastErrorCode() << std::endl;
        delete [] m_data;
        m_data = nullptr;
        return nullptr;
    }

    PHYSFS_close(myfile);

    SDL_RWops *rw = SDL_RWFromMem (m_data, m_size);

    return Mix_LoadMUS_RW(rw, 0);
};

Mix_Chunk* Load_Wav(const char* path) {
    if (!PHYSFS_exists(path))
    {
        std::cout << "Unable to find " << path << std::endl;
        return nullptr; //file doesn't exist
    }

    PHYSFS_file* myfile = PHYSFS_openRead(path);

    // Get the lenght of the file
    auto m_size = PHYSFS_fileLength(myfile);

    // Get the file data.
    auto m_data = new uint8_t[m_size];

    auto length_read = PHYSFS_readBytes(myfile, m_data, static_cast<PHYSFS_uint64>(m_size));

    if (length_read != m_size)
    {
        std::cout << PHYSFS_getLastErrorCode() << std::endl;
        delete [] m_data;
        m_data = nullptr;
        return nullptr;
    }

    PHYSFS_close(myfile);

    SDL_RWops *rw = SDL_RWFromMem (m_data, m_size);

    return Mix_LoadWAV_RW(rw, 0);
}