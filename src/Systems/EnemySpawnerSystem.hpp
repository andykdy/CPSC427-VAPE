//
// Created by Cody on 10/17/2019.
//

#ifndef VAPE_ENEMYSPAWNERSYSTEM_HPP
#define VAPE_ENEMYSPAWNERSYSTEM_HPP

#include <Engine/ECS/System.hpp>
#include <Engine/ECS/Entity.hpp>
#include <Entities/Enemies/turtle.hpp>
#include <Levels/Levels.hpp>

class EnemySpawnerSystem : public ECS::System {
private:
    int time = 0;
    std::vector<Enemy*> enemies;
    Levels::Timeline level = Levels::level1Timeline;
public:
    void update(float ms) override;
    std::vector<Enemy*> *getEnemies();
    void reset(Levels::Timeline& levelTimeline);
    void spawnWave(const Levels::Wave& wave);
};


#endif //VAPE_ENEMYSPAWNERSYSTEM_HPP
