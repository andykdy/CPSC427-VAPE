//
// Created by Cody on 10/16/2019.
//

#include <common.hpp>
#include <Engine/ECS/ECS.hpp>
#include <cmath>

#ifndef VAPE_TRANSFORMCOMPONENT_HPP
#define VAPE_TRANSFORMCOMPONENT_HPP

class TransformComponent : public ECS::Component {
public:
    mat3 out;

    void begin()
    {
        out = { { 1.f, 0.f, 0.f }, { 0.f, 1.f, 0.f}, { 0.f, 0.f, 1.f} };
    }

    void scale(vec2 scale)
    {
        mat3 S = { { scale.x, 0.f, 0.f },{ 0.f, scale.y, 0.f },{ 0.f, 0.f, 1.f } };
        out = mul(out, S);
    }

    void rotate(float radians)
    {
        float c = cosf(radians);
        float s = sinf(radians);
        mat3 R = { { c, s, 0.f },{ -s, c, 0.f },{ 0.f, 0.f, 1.f } };
        out = mul(out, R);
    }

    void translate(vec2 offset)
    {
        mat3 T = { { 1.f, 0.f, 0.f },{ 0.f, 1.f, 0.f },{ offset.x, offset.y, 1.f } };
        out = mul(out, T);
    }

    void end()
    {
        //
    }
};

#endif //VAPE_TRANSFORMCOMPONENT_HPP
