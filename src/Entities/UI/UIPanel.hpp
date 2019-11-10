//
// Created by Cody on 11/8/2019.
//

#ifndef VAPE_UIPANEL_HPP
#define VAPE_UIPANEL_HPP


#include <Engine/ECS/Entity.hpp>

class UIPanel: public ECS::Entity {
private:
    vec2 m_screen;
public:
    bool init(vec2 screen, float height);
    void draw(const mat3& projection) override;
    void destroy() override;
};


#endif //VAPE_UIPANEL_HPP
