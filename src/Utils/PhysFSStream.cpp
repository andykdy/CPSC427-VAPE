//
// Created by Cody on 11/29/2019.
//

#include <iostream>
#include "PhysFSStream.hpp"

PhysFSStream::PhysFSStream(const std::string &fileName) {
    load(fileName);
}

PhysFSStream::~PhysFSStream() {
    if (file != nullptr)
    {
        PHYSFS_close(file);
    }
}

bool PhysFSStream::load(const std::string &fileName) {
    if (!PHYSFS_exists(fileName.c_str()))
    {
        std::cout << "Unable to find " << fileName << std::endl;
        return false;
    }

    file = PHYSFS_openRead(fileName.c_str());

    return (file != nullptr);
}

int64_t PhysFSStream::read(void *data, int64_t size) {
    return PHYSFS_readBytes(file, data, (PHYSFS_uint64)size);
}

int64_t PhysFSStream::seek(int64_t position) {
    if (PHYSFS_seek(file, position) == 0)
    {
        return -1;
    }
    return position;
}

int64_t PhysFSStream::getSize() {
    return PHYSFS_fileLength(file);
}
