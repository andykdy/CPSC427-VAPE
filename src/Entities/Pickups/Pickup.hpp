//
// Created by Cody on 11/10/2019.
//

#ifndef VAPE_PICKUP_HPP
#define VAPE_PICKUP_HPP

#include <Engine/ECS/Entity.hpp>
#include <Entities/Player.hpp>
#include <Engine/States/LevelState.hpp>

// Forward declarations
class Player;
class LevelState;

class Pickup : public ECS::Entity {
public:
    virtual bool init(vec2 position) = 0;
    virtual vec2 get_position() const = 0;
    virtual bool collides_with(const Player &player) = 0;
    virtual void applyEffect(LevelState& level) = 0;
    virtual bool isOffScreen(const vec2& screen) = 0;
};

#endif //VAPE_PICKUP_HPP
