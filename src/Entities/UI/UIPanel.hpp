//
// Created by matte on 2019-11-12.
//

#ifndef VAPE_UIPANEL_HPP
#define VAPE_UIPANEL_HPP


#include <Engine/ECS/Entity.hpp>

class UIPanel: public ECS::Entity {
private:
    vec2 m_screen;
public:
    bool init(vec2 screen, float height, float width);
    void draw(const mat3& projection) override;
    void destroy() override;

    static Texture UI_texture;
};


#endif //VAPE_UIPANEL_HPP
