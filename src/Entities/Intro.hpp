//
// Created by Cody on 11/10/2019.
//

#ifndef VAPE_INTRO_HPP
#define VAPE_INTRO_HPP

#include "common.hpp"
#include "Video.hpp"
#include <Engine/ECS/ECS.hpp>

// TODO: this could maybe be refactored out into a "slideshow" class, and have it take in a set of textures+durations+music. Then use if for potention in-between cutscenes and ending

class Intro : public ECS::Entity {
public:
    bool init(vec2 screen);
    void update(float ms) override;
    void draw(const mat3& projection) override;
    void destroy() override;

private:
    int m_part = 0;
    float m_timer;
    std::vector<Texture*> introTextures;
};


#endif //VAPE_INTRO_HPP
