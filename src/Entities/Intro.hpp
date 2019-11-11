//
// Created by Cody on 11/10/2019.
//

#ifndef VAPE_INTRO_HPP
#define VAPE_INTRO_HPP

#include "common.hpp"
#include <Engine/ECS/ECS.hpp>

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
