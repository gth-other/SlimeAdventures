#include "Skeleton.hpp"
#include "Axeman.hpp"
#include "Gunslinger.hpp"


#pragma once


class Enemies {
public:
    Enemies();
    Enemies(const std::string& path);

    void add(const Enemies& enemies);
    void clear();

    void save(const std::string& path);

    std::list<Enemy*> _enemies;
private:
    std::list<Skeleton> _skeletons;
    std::list<Axeman> _axemen;
    std::list<Gunslinger> _gunslingers;
};