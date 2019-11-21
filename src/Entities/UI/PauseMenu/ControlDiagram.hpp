//
// Created by Cody on 11/19/2019.
//

#ifndef VAPE_CONTROLDIAGRAM_HPP
#define VAPE_CONTROLDIAGRAM_HPP


#include <Engine/ECS/Entity.hpp>

class ControlDiagram : public ECS::Entity {
    static Texture controls_texture;
public:
    bool init(vec2 position, vec2 scale, float rotation);
    void destroy() override;
    void update(float ms) override;
    void draw(const mat3 &projection) override;
};


#endif //VAPE_CONTROLDIAGRAM_HPP
