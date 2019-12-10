//
// Created by Cody on 11/20/2019.
//

#ifndef VAPE_TEXT_HPP
#define VAPE_TEXT_HPP


#include <Engine/Graphics/Font.hpp>
#include <vector>

class Text : public EntityOld {
private:
    std::string m_text;
    std::vector<TexturedVertex> vertices{};
    std::vector<uint16_t> indices{};
protected:
    Font* m_font;
    vec3 m_color;
    float m_alpha;
    vec2 m_scale;
public:
    virtual bool init(Font* font);
    void clear();
    void destroy();
    void draw(const mat3& projection);
    void setText(const char* text, float z = -0.05f);

    void setPosition(const vec2& pos) {
        motion.position = pos;
    }

    void setColor(const vec3& color) {
        m_color = color;
    }

    void setAlpha(const float alpha) {
        m_alpha = alpha;
    }

    void setScale(const vec2 scale) {
        m_scale = scale;
    }

    void scroll_up(float ms);

    vec2 getBoundingBox();
};


#endif //VAPE_TEXT_HPP
