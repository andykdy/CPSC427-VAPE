//
// Created by Cody on 11/6/2019.
//

#ifndef VAPE_COLLISIONSYSTEM_HPP
#define VAPE_COLLISIONSYSTEM_HPP


#include <Engine/ECS/System.hpp>

class CollisionSystem : public ECS::System {
private:

public:
    void update(float ms) override;

private:
    bool checkCollision(std::unique_ptr<ECS::Entity>& e1, std::unique_ptr<ECS::Entity>& e2);
};


#endif //VAPE_COLLISIONSYSTEM_HPP
