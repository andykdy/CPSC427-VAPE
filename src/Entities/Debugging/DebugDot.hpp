//
// Created by Cody on 11/11/2019.
//

#ifndef VAPE_DEBUGDOT_HPP
#define VAPE_DEBUGDOT_HPP


#include <common.hpp>
#include <vector>

class DebugDot : EntityOld {
public:
    bool init();
    void destroy();
    void draw(const mat3 &projection) override;
    void draw(const mat3& projection, vec3 color, vec2 pos, float rad = 0, vec2 scale = {1,1});
    void draw(const mat3& projection, vec3 color, std::vector<vec2> path, float rad = 0, vec2 scale = {1,1});
};


#endif //VAPE_DEBUGDOT_HPP
