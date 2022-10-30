#include "Gunslinger.hpp"


Gunslinger::Gunslinger() = default;
Gunslinger::Gunslinger(float x, float y) {
    this->_hitbox = {x, y, 50, 64};
    this->_t0 = 1e+9;
    this->_vx = 0;
    this->_vy = 0;
    this->_animation_ctr = 0;
    this->_right = true;
    this->_alive = true;
    this->_dt1 = 0;
}
void Gunslinger::draw(Camera camera, SDL_Renderer *renderer) const {
    if (!Gunslinger::DataWasLoaded) {
        Gunslinger::Textures = IMG_LoadTexture(renderer, LinkManager::path("data/images/characters/gunslinger.png").c_str());
        Gunslinger::Shoot = Mix_LoadWAV(LinkManager::path("data/sounds/shot.mp3").c_str());
        Gunslinger::Death = Mix_LoadWAV(LinkManager::path("data/sounds/rifle.mp3").c_str());
        Gunslinger::DataWasLoaded = true;
    }

    SDL_Rect screct = {64 * (int32_t)std::floor(this->_animation_ctr), 64 * !this->_right, 64, 64};
    SDL_Rect dstrect = {(int32_t)(this->_hitbox._x - 7 - camera.display()._x), (int32_t)(this->_hitbox._y - camera.display()._y), 64, 64};

    SDL_RenderCopy(renderer, Gunslinger::Textures, &screct, &dstrect);
}
void Gunslinger::update(Rect player_hitbox, std::list<Bullet> &bullets, const Map &map) {
    float dt = this->_timer();
    float dt_part;

    float air_factor1;

    for (int32_t i = 0; i <= (int32_t)(dt / Gunslinger::MaximalUpdateDelay); i = i + 1) {
        dt_part = Gunslinger::MaximalUpdateDelay;
        if (i == (int32_t)(dt / Gunslinger::MaximalUpdateDelay)) dt_part = dt - (float)i * Gunslinger::MaximalUpdateDelay;

        this->_dt1 = this->_dt1 + dt_part;
        air_factor1 = this->_get_air_factor(map, Gunslinger::air_factor);

        if (this->_possible_to_shoot(player_hitbox, map)) {
            this->_right = (player_hitbox._x > this->_hitbox._x);

            this->_stop(dt_part, air_factor1, Gunslinger::ax);

            if (this->_dt1 >= Gunslinger::shoot_delay) {
                bullets.emplace_front(this->_hitbox._x + this->_hitbox._w / 2, this->_hitbox._y + this->_hitbox._h / 2, (player_hitbox._x + player_hitbox._w / 2) - (this->_hitbox._x + this->_hitbox._w / 2), (player_hitbox._y + player_hitbox._h / 2) - (this->_hitbox._y + this->_hitbox._h / 2));
                this->_dt1 = 0;
                Mix_PlayChannel(-1, Gunslinger::Shoot, 0);
            }
        }
        else {
            this->_necessary_direction(this->_right, map);

            if (this->_right) this->_move_right(dt_part, air_factor1, Gunslinger::ax, Gunslinger::vx_max);
            else this->_move_left(dt_part, air_factor1, Gunslinger::ax, Gunslinger::vx_max);
        }

        this->_gravity(dt_part, Gunslinger::ay);

        this->_update_x(dt_part, map);
        this->_update_y(dt_part, map);

        this->_animation_ctr = this->_animation_ctr + dt_part * 12;
        if (this->_animation_ctr >= 4) this->_animation_ctr = 0;
    }
}
void Gunslinger::kill() {
    if (this->_alive) {
        Mix_PlayChannel(-1, Gunslinger::Death, 0);
        this->disappear();
    }
}
bool Gunslinger::_possible_to_shoot(Rect player_hitbox, const Map &map) {
    auto x0 = (int32_t)(this->_hitbox._x + this->_hitbox._w / 2);
    auto y0 = (int32_t)(this->_hitbox._y + this->_hitbox._h / 2);

    auto x1 = (int32_t)(player_hitbox._x + player_hitbox._w / 2);
    auto y1 = (int32_t)(player_hitbox._y + player_hitbox._h / 2);

    if (std::sqrt(std::pow(x0 - x1, 2) + std::pow(y0 - y1, 2)) >= 10000) return false;

    int32_t tx0 = (int32_t)x0 / 32;
    int32_t ty0 = (int32_t)y0 / 32;

    int32_t tx1 = (int32_t)x1 / 32;
    int32_t ty1 = (int32_t)y1 / 32;

    SDL_Rect rect;

    for (int32_t ty = std::min(ty0, ty1); ty <= std::max(ty0, ty1); ty = ty + 1) {
        for (int32_t tx = std::min(tx0, tx1); tx <= std::max(tx0, tx1); tx = tx + 1) {
            if (map.solid(ty, tx) or map.dangerous(ty, tx)) {
                rect = {32 * tx, 32 * ty, 32, 32};
                if (SDL_IntersectRectAndLine(&rect, &x0, &y0, &x1, &y1)) return false;
            }
        }
    }

    return true;
}