#include "Character.hpp"


Rect Character::hitbox() const {
    return this->_hitbox;
}
bool Character::right() const {
    return this->_right;
}
void Character::disappear() {
    this->_alive = false;
}
bool Character::alive() const {
    return this->_alive;
}
float Character::_timer() {
    float t1 = (float)SDL_GetTicks() / 1000.f;
    float dt = std::max(0.f, t1 - this->_t0);
    this->_t0 = t1;

    return dt;
}
float Character::_get_air_factor(const Map &map, float air_factor) {
    float air_factor1 = 1;
    this->_hitbox._y = this->_hitbox._y + 2;
    if (!this->_in_solid(map)) air_factor1 = air_factor;
    this->_hitbox._y = this->_hitbox._y - 2;

    return air_factor1;
}
void Character::_stop(float dt, float air_factor, float ax) {
    if (this->_vx > 0) this->_vx = std::max(0.f, this->_vx - dt * air_factor * ax);
    else this->_vx = std::min(0.f, this->_vx + dt * air_factor * ax);
}
void Character::_move_left(float dt, float air_factor, float ax, float vx_max) {
    this->_vx = std::max(-vx_max, this->_vx - dt * air_factor * ax);
    this->_right = false;
}
void Character::_move_right(float dt, float air_factor, float ax, float vx_max) {
    this->_vx = std::min(vx_max, this->_vx + dt * air_factor * ax);
    this->_right = true;
}
void Character::_jump(float vy_jump, const Map &map) {
    this->_hitbox._y = this->_hitbox._y + 2;
    if (this->_in_solid(map)) this->_vy = vy_jump;
    this->_hitbox._y = this->_hitbox._y - 2;
}
void Character::_gravity(float dt, float ay) {
    this->_vy = this->_vy + dt * ay;
}
void Character::_update_x(float dt, const Map &map) {
    this->_hitbox._x = this->_hitbox._x + dt * this->_vx;

    if (this->_hitbox._x < 0 or this->_hitbox._x >= 32 * (float)(map.tx() - 1)) {
        this->disappear();
        this->_hitbox._x = this->_hitbox._x - dt * this->_vx;
    }
    else if (this->_in_dangerous(map)) this->kill();
    else if (this->_in_solid(map)) {
        this->_hitbox._x = this->_hitbox._x - dt * this->_vx;
        this->_vx = 0;
    }
}
void Character::_update_y(float dt, const Map &map) {
    this->_hitbox._y = this->_hitbox._y + dt * this->_vy;

    if (this->_hitbox._y < 0 or this->_hitbox._y >= 32 * (float)(map.ty() - 1)) {
        this->disappear();
        this->_hitbox._y = this->_hitbox._y - dt * this->_vy;
    }
    else if (this->_in_dangerous(map)) this->kill();
    else if (this->_in_solid(map)) {
        this->_hitbox._y = this->_hitbox._y - dt * this->_vy;
        this->_vy = 0;
    }
}
bool Character::_in_solid(const Map &map) const {
    auto tw = (int32_t)std::ceil(this->_hitbox._w / 32);
    auto th = (int32_t)std::ceil(this->_hitbox._h / 32);

    int32_t from_ty = std::max(0, (int32_t)this->_hitbox._y / 32 - th);
    int32_t to_ty = std::min(map.ty() - th, (int32_t)this->_hitbox._y / 32 + th);

    int32_t from_tx = std::max(0, (int32_t)this->_hitbox._x / 32 - tw);
    int32_t to_tx = std::min(map.tx() - tw, (int32_t)this->_hitbox._x / 32 + tw);

    Rect tile_hitbox;

    for (int32_t ty = from_ty; ty <= to_ty; ty = ty + 1) {
        for (int32_t tx = from_tx; tx <= to_tx; tx = tx + 1) {
            if (map.solid(ty, tx)){
                tile_hitbox = {32 * (float)tx, 32 * (float)ty, 32, 32};
                if (this->_hitbox && tile_hitbox) return true;
            }
        }
    }

    return false;
}
bool Character::_in_dangerous(const Map &map) const {
    auto tw = (int32_t)std::ceil(this->_hitbox._w / 32);
    auto th = (int32_t)std::ceil(this->_hitbox._h / 32);

    int32_t from_ty = std::max(0, (int32_t)this->_hitbox._y / 32 - th);
    int32_t to_ty = std::min(map.ty() - th, (int32_t)this->_hitbox._y / 32 + th);

    int32_t from_tx = std::max(0, (int32_t)this->_hitbox._x / 32 - tw);
    int32_t to_tx = std::min(map.tx() - tw, (int32_t)this->_hitbox._x / 32 + tw);

    Rect tile_hitbox;

    for (int32_t ty = from_ty; ty <= to_ty; ty = ty + 1) {
        for (int32_t tx = from_tx; tx <= to_tx; tx = tx + 1) {
            if (map.dangerous(ty, tx)){
                // Для более комфортной игры хитбоксы враждебных плиток ужимаются (лучше, чтобы персонаж выжил, когда должен умереть, чем умер, когда должен выжить).
                tile_hitbox = {32 * (float)tx + 8, 32 * (float)ty + 8, 16, 16};
                if (this->_hitbox && tile_hitbox) return true;
            }
        }
    }

    return false;
}