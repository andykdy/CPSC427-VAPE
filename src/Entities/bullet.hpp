//
// Created by Cody on 9/2/2019.
//

#ifndef INC_436D_BULLET_H
#define INC_436D_BULLET_H


#include "common.hpp"
#include "turtle.hpp"
#include "fish.hpp"

// Forward declarations
class Player;
class Boss1;

class Bullet : public Entity {
    // Shared between all bullets, no need to load one for each instance
    static Texture bullet_texture;

public:
    bool init(vec2 position, float rotation);
    void destroy();
    void update(float ms);
    void draw(const mat3& projection)override;

    // Returns the current bullet position
    vec2 get_position()const;

    // Collision routines for player, turtles and fish
    bool collides_with(const Player& player);
    bool collides_with(const Turtle& turtle);
    bool collides_with(const Fish& fish);
    bool collides_with(const Boss1& boss); //TODO either generic collides, or generic boss/enemy

    // Returns the bullet' bounding box for collision detection, called by collides_with()
    vec2 get_bounding_box()const;

private:
};


#endif //INC_436D_BULLET_H
