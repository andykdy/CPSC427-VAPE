//
// Created by matte on 2019-12-06.
//

#ifndef VAPE_SCOREBACKGROUND_HPP
#define VAPE_SCOREBACKGROUND_HPP

#include <Engine/ECS/Entity.hpp>

class ScoreBackground: public ECS::Entity {
private:
    vec2 m_screen;
public:
    bool init(vec2 screen);
    void draw(const mat3& projection) override;
    void destroy() override;

};

#endif //VAPE_SCOREBACKGROUND_HPP
