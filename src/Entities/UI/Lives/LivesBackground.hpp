//
// Created by matte on 2019-12-06.
//

#ifndef VAPE_LIVESBACKGROUND_HPP
#define VAPE_LIVESBACKGROUND_HPP

#include <Engine/ECS/Entity.hpp>

class LivesBackground: public ECS::Entity {
private:
    vec2 m_screen;
public:
    bool init(vec2 screen);
    void draw(const mat3& projection) override;
    void destroy() override;

};

#endif //VAPE_LIVESBACKGROUND_HPP
