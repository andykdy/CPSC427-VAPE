//
// Created by Cody on 10/20/2019.
//

#ifndef VAPE_SCREENBOUNDARYCOMPONENT_HPP
#define VAPE_SCREENBOUNDARYCOMPONENT_HPP

#include <Engine/ECS/Component.hpp>

class BoundaryComponent : public ECS::Component {
public:
    float minX = 0;
    float maxX = 0;
    float minY = 0;
    float maxY = 0;

    BoundaryComponent(float minX, float maxX, float minY, float maxY) : minX(minX), maxX(maxX), minY(minY), maxY(maxY) {}
};

#endif //VAPE_SCREENBOUNDARYCOMPONENT_HPP
