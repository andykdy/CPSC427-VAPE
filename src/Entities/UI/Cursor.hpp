//
// Created by Cody on 11/14/2019.
//

#ifndef VAPE_CURSOR_HPP
#define VAPE_CURSOR_HPP


#include <Engine/ECS/Entity.hpp>

class Cursor : public ECS::Entity {
    static Texture cursor_texture;
public:
    bool init(const vec2& position, const vec2&scale, float rotation);
    void destroy() override;
    void draw(const mat3 &projection) override;
    void setPosition(const vec2& position);
};


#endif //VAPE_CURSOR_HPP
