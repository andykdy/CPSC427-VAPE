//
// Created by Cody on 11/29/2019.
//

#ifndef VAPE_PHYSFSSTREAM_HPP
#define VAPE_PHYSFSSTREAM_HPP


#include <physfs.h>
#include <string>

class PhysFSStream {
    private:
    PHYSFS_File* file{ nullptr };

    public:
    PhysFSStream(const std::string& fileName);
    ~PhysFSStream();

    bool load(const std::string& fileName);

    int64_t read(void* data, int64_t size);
    int64_t seek(int64_t position);
    int64_t getSize();

    const char* getError() const
    {
        return PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode());
    }
};


#endif //VAPE_PHYSFSSTREAM_HPP
