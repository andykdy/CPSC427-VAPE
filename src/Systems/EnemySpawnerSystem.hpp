//
// Created by Cody on 10/17/2019.
//

#ifndef VAPE_ENEMYSPAWNERSYSTEM_HPP
#define VAPE_ENEMYSPAWNERSYSTEM_HPP

#include <Engine/ECS/System.hpp>
#include <Engine/ECS/Entity.hpp>
#include <Entities/turtle.hpp>
#include <Levels/Level.hpp>

class EnemySpawnerSystem : public ECS::System {
private:
    int time = 0;
    std::vector<ECS::Entity*> enemies;
    Levels::Timeline level = Levels::level1; // TODO
public:
    void update(float ms) override;
    std::vector<ECS::Entity*> *getEnemies();
    void reset();
};


#endif //VAPE_ENEMYSPAWNERSYSTEM_HPP
