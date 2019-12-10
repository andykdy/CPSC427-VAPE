//
// Created by Cody on 11/20/2019.
//

#include <vector>
#include "Font.hpp"

#include <stb_truetype.h>
#include <iostream>
#include <physfs.h>
#include <sstream>

unsigned char fontdata[1 << 20];
unsigned char temp_bitmap[512 * 512];

Font::Font(const char *path) {
    m_path = std::string(path);

    PHYSFS_file* myfile = PHYSFS_openRead(path);
    auto m_size = PHYSFS_fileLength(myfile);
    auto length_read = PHYSFS_readBytes(myfile, fontdata, 1<<20);

    if (length_read != m_size)
    {
        std::stringstream ss;
        ss << PHYSFS_getLastErrorCode() << std::endl;
        throw std::runtime_error(ss.str().c_str());
    }
    
    PHYSFS_close(myfile);
    stbtt_BakeFontBitmap(fontdata,stbtt_GetFontOffsetForIndex(fontdata, 0), 32.0, temp_bitmap,512,512, 32,96, cdata);
    // can free ttf_buffer at this point
    glGenTextures(1, &m_tex);
    glBindTexture(GL_TEXTURE_2D, m_tex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 512,512, 0, GL_RED, GL_UNSIGNED_BYTE, temp_bitmap);
    // can free temp_bitmap at this point
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
}

Font::~Font() {
    glDeleteTextures(1, &m_tex);

}
