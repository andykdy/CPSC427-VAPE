//
// Created by Cody on 11/7/2019.
//

#include "Laser.hpp"

bool Laser::init(vec2 position, float rotation) {
    return false;
}

void Laser::destroy() {
    Entity::destroy();
}

void Laser::update(float ms) {
    Entity::update(ms);
}

void Laser::draw(const mat3 &projection) {
    Entity::draw(projection);
}

vec2 Laser::get_position() const {
    return vec2();
}

bool Laser::collides_with(const Player &player) {
    return false;
}

bool Laser::collides_with(const Turtle &turtle) {
    return false;
}

bool Laser::collides_with(const Boss &boss) {
    return false;
}

bool Laser::isOffScreen(const vec2 &screen) {
    return false;
}
