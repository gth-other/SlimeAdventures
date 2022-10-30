#include "Enemy.hpp"


#pragma once


class Gunslinger : public Enemy {
    // Стрелок.
    // Перемещается быстрее игрока.
    // Если игрок в зоне видимости, то останавливается и стреляет. Если нет - ходит от одного препятствия до другого.
public:
    Gunslinger();
    Gunslinger(float x, float y);

    void draw(Camera camera, SDL_Renderer *renderer) const override;
    void update(Rect player_hitbox, std::list<Bullet> &bullets, const Map &map) override;

    void kill() override;
private:
    float _dt1;

    static constexpr float ax = 40 * 32;
    static constexpr float vx_max = 10 * 32;
    static constexpr float air_factor = 0;

    static constexpr float ay = 10 * 32;

    static constexpr float shoot_delay = 3;

    inline static SDL_Texture *Textures;
    inline static Mix_Chunk *Shoot;
    inline static Mix_Chunk *Death;
    inline static bool DataWasLoaded = false;

    [[nodiscard]] bool _possible_to_shoot(Rect player_hitbox, const Map &map);
};