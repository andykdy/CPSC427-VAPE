//
// Created by Cody on 11/26/2019.
//

#include "Boss3.hpp"

bool Boss3::init(vec2 screen) {
    // TODO
    Boss::addDamage(1000); // TODO remove
    return true;
}

void Boss3::destroy() {
    // TODO
    ECS::Entity::destroy();
}

void Boss3::update(float ms) {
    // TODO
}

void Boss3::draw(const mat3 &projection) {
    // TODO
}

vec2 Boss3::get_position() const {
    return vec2(); // TODO
}

void Boss3::set_position(vec2 position) {
    // TODO
}

vec2 Boss3::get_bounding_box() const {
    return vec2();
}

void Boss3::addDamage(int damage) {
    Boss::addDamage(damage);
}

bool Boss3::collidesWith(const Vamp &vamp) {
    return checkCollision(vamp.get_position(), vamp.get_bounding_box());
}

bool Boss3::collidesWith(Player &player) {
    return checkCollision(player.get_position(), player.get_bounding_box());
}

bool Boss3::checkCollision(vec2 pos, vec2 box) const {
    // TODO
    return false;
}
