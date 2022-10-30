#include <list>
#include "Bullet.hpp"


#pragma once


class Enemy : public Character {
public:
    virtual void update(Rect player_hitbox, std::list<Bullet> &bullets, const Map &map) = 0;
protected:
    void _necessary_direction(bool &right, const Map &map);
};