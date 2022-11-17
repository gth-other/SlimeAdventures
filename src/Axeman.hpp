#include "Enemy.hpp"


#pragma once


class Axeman : public Enemy {
    // Лесоруб.
    // Перемещается быстрее игрока.
    // Если игрок в зоне видимости, то преследует его. Если нет - ходит от одного препятствия до другого.
public:
    Axeman();
    Axeman(float x, float y, float vx = 0, float vy = 0);

    void draw(Camera camera, SDL_Renderer *renderer) const override;
    void update(Rect player_hitbox, std::list<Bullet> &bullets, const Map &map) override;

    void kill() override;
private:
    static constexpr float ax = 40 * 32;
    static constexpr float vx_max = 10 * 32;
    static constexpr float air_factor = 0;

    static constexpr float ay = 10 * 32;

    inline static SDL_Texture *Textures;
    inline static Mix_Chunk *Death;
    inline static bool DataWasLoaded = false;

    [[nodiscard]] bool _possible_to_get_to_player(Rect player_hitbox, const Map &map) const;
};