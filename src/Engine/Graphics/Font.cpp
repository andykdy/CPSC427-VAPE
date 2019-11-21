//
// Created by Cody on 11/20/2019.
//

#include <vector>
#include "Font.hpp"

#include <stb_truetype.h>
#include <iostream>

Font::Font(const char *path) {
    m_path = std::string(path);

    unsigned char fontdata[1<<20];
    unsigned char temp_bitmap[512*512];

    fread(fontdata, 1, 1<<20, fopen(path, "rb"));
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
