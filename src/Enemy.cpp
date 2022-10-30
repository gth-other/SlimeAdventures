#include "Enemy.hpp"


void Enemy::_necessary_direction(bool &right, const Map &map) {
    bool original = right;

    this->_hitbox._x = this->_hitbox._x - 1.5f * this->_hitbox._w;
    if (this->_in_solid(map)) right = true;
    this->_hitbox._y = this->_hitbox._y + 2;
    if (!this->_in_solid(map)) right = true;
    if (this->_in_dangerous(map)) right = true;
    this->_hitbox._y = this->_hitbox._y - 2;
    this->_hitbox._x = this->_hitbox._x + 1.5f * this->_hitbox._w;
    if (right != original) return;

    this->_hitbox._x = this->_hitbox._x + 1.5f * this->_hitbox._w;
    if (this->_in_solid(map)) right = false;
    this->_hitbox._y = this->_hitbox._y + 2;
    if (!this->_in_solid(map)) right = false;
    if (this->_in_dangerous(map)) right = false;
    this->_hitbox._y = this->_hitbox._y - 2;
    this->_hitbox._x = this->_hitbox._x - 1.5f * this->_hitbox._w;
    if (right != original) return;
}