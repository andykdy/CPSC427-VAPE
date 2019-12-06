//
// Created by matte on 2019-12-05.
//

#include "ScoreText.hpp"


namespace {
    const size_t TEXT_SCROLL_SPEED = 100;
    const size_t TEXT_LIFE = 1000;
}

bool ScoreText::init(Font *font) {
    // Loading shaders
    if (!effect.load_from_file(shader_path("font.vs.glsl"), shader_path("font.fs.glsl")))
        throw std::runtime_error("Failed to load font textured shader");

    m_font = font;
    setText("");
    m_color = {1.f,1.f,1.f};
    m_life = TEXT_LIFE;

    return true;

}


void ScoreText::scroll_up(float ms)
{
    float step = TEXT_SCROLL_SPEED * (ms / 1000);
    motion.position.y -= step;
    m_life -= ms;
}

bool ScoreText::is_alive() {
    return m_life > 0;
}