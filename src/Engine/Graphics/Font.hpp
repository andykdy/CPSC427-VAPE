//
// Created by Cody on 11/20/2019.
//

#ifndef VAPE_FONT_HPP
#define VAPE_FONT_HPP

#include "stb_truetype.h"
#include "common.hpp"

class Font : public EntityOld {
private:
    stbtt_bakedchar cdata[96]; // ASCII 32..126 is 95 glyphs
    GLuint m_tex = 0;
    std::vector<TexturedVertex> vertices{};
    std::vector<uint16_t> indices{};
    vec3 m_color;
public:
    explicit Font(const char* path);

    void draw(const mat3& projection) override;

    void setText(const char* text, float z = -0.05f);

    void setPosition(const vec2& pos) {
        motion.position = pos;
    }

    void setColor(const vec3& color) {
        m_color = color;
    }

    void clear();

    virtual ~Font();
};


#endif //VAPE_FONT_HPP
