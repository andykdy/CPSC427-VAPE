//
// Created by Cody on 10/17/2019.
//

#ifndef VAPE_ENEMYSPAWNERSYSTEM_HPP
#define VAPE_ENEMYSPAWNERSYSTEM_HPP

#include <Engine/ECS/System.hpp>
#include <Entities/turtle.hpp>
#include <Levels/Levels.hpp>

class EnemySpawnerSystem : public ECS::System {
private:
    int time = 0;
    std::vector<Turtle*> enemies = {};
    Levels::Timeline level = Levels::level1Timeline; // TODO
public:
    void update(float ms) override;
    std::vector<Turtle*> *getEnemies();
    void reset(Levels::Timeline levelTimeline);
};


#endif //VAPE_ENEMYSPAWNERSYSTEM_HPP
