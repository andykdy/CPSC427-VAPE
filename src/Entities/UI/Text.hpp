//
// Created by Cody on 11/20/2019.
//

#ifndef VAPE_TEXT_HPP
#define VAPE_TEXT_HPP


#include <Engine/Graphics/Font.hpp>
#include <vector>

class Text : public EntityOld {
private:
    Font* m_font;
    std::string m_text;
    vec3 m_color;
    float m_life;
    std::vector<TexturedVertex> vertices{};
    std::vector<uint16_t> indices{};
public:
    bool init(Font* font);
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

    bool is_alive() {
        return m_life > 0;
    }

    void scroll_up(float ms);

    vec2 getBoundingBox();
};


#endif //VAPE_TEXT_HPP
