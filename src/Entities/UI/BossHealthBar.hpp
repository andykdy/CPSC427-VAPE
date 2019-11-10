//
// Created by Cody on 11/9/2019.
//

#ifndef VAPE_BOSSHEALTHBAR_HPP
#define VAPE_BOSSHEALTHBAR_HPP


#include <Engine/ECS/Entity.hpp>

class BossHealthBar : public ECS::Entity {
    static Texture health_bar_texture; // TODO texture or just box/outline

public:
    bool init(vec2 screen);
    void update(float ms) override {};
    void draw(const mat3& projection) override;
    void destroy() override;

    vec2 getSize() { return m_size; };
private:
    vec2 m_size = {100, 100}; // TODO temp
};


#endif //VAPE_BOSSHEALTHBAR_HPP
