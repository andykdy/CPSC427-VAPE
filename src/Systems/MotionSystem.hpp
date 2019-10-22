//
// Created by Cody on 10/20/2019.
//

#ifndef VAPE_MOTIONSYSTEM_HPP
#define VAPE_MOTIONSYSTEM_HPP


#include <Engine/ECS/System.hpp>
#include <Components/MotionComponent.hpp>

class MotionSystem : public ECS::System {
private:
    void accelerate(MotionComponent* motion);
public:
    void update(float ms) override;
};


#endif //VAPE_MOTIONSYSTEM_HPP
