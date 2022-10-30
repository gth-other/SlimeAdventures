#include "Map.hpp"


#pragma once


class Character {
public:
    virtual void draw(Camera camera, SDL_Renderer *renderer) const = 0;

    [[nodiscard]] Rect hitbox() const;

    [[nodiscard]] bool right() const;

    virtual void kill() = 0;
    void disappear();
    [[nodiscard]] bool alive() const;
protected:
    // Размер, а следовательно и положение хитбокса не равны размеру и положению текстуры!
    Rect _hitbox;
    float _t0;
    float _vx;
    float _vy;
    float _animation_ctr;
    bool _right;
    bool _alive;

    float _timer();

    [[nodiscard]] float _get_air_factor(const Map &map, float air_factor);

    void _stop(float dt, float air_factor, float ax);
    void _move_left(float dt, float air_factor, float ax, float vx_max);
    void _move_right(float dt, float air_factor, float ax, float vx_max);
    void _jump(float vy_jump, const Map &map);

    void _gravity(float dt, float ay);

    void _update_x(float dt, const Map &map);
    void _update_y(float dt, const Map &map);

    [[nodiscard]] bool _in_solid(const Map &map) const;
    [[nodiscard]] bool _in_dangerous(const Map &map) const;
};