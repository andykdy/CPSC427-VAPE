//
// Created by Cody on 11/20/2019.
//

#ifndef VAPE_FONT_HPP
#define VAPE_FONT_HPP

#include "stb_truetype.h"
#include "common.hpp"

class Font {
private:
    std::string m_path;
    stbtt_bakedchar cdata[96]; // ASCII 32..126 is 95 glyphs
    GLuint m_tex = 0;
public:
    explicit Font(const char* path);

    const GLuint getTexture() { return m_tex; };

    const stbtt_bakedchar *getCdata() const { return cdata; }

    virtual ~Font();
};


#endif //VAPE_FONT_HPP
