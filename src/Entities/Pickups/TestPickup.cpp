//
// Created by Cody on 11/10/2019.
//

#include "TestPickup.hpp"


bool TestPickup::init(vec2 position) {
    return false;
}

void TestPickup::update(float ms) {
    Entity::update(ms);
}

void TestPickup::draw(const mat3 &projection) {
    Entity::draw(projection);
}

void TestPickup::destroy() {
    Entity::destroy();
}

vec2 TestPickup::get_position() const {
    return vec2();
}

bool TestPickup::collides_with(const Player &player) {
    return false;
}

void TestPickup::applyEffect(LevelState& level) {

}

bool TestPickup::isOffScreen(const vec2 &screen) {
    return false;
}
