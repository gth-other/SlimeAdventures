#include "Enemy.hpp"


#pragma once


class Skeleton : public Enemy {
    // Скелет.
    // Перемещается со скоростью игрока.
    // Ходит от одного препятствия до другого.
public:
    Skeleton();
    Skeleton(float x, float y, float vx = 0, float vy = 0);

    void draw(Camera camera, SDL_Renderer *renderer) const override;
    void update(Rect player_hitbox, std::list<Bullet> &bullets, const Map &map) override;

    void kill() override;
private:
    static constexpr float ax = 24 * 32;
    static constexpr float vx_max = 6 * 32;
    static constexpr float air_factor = 0;

    static constexpr float ay = 10 * 32;

    inline static SDL_Texture *Textures;
    inline static Mix_Chunk *Death;
    inline static bool DataWasLoaded = false;
};