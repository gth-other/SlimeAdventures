#include "Axeman.hpp"


Axeman::Axeman() = default;
Axeman::Axeman(float x, float y) {
    this->_hitbox = {x, y, 40, 60};
    this->_t0 = 1e+9;
    this->_vx = 0;
    this->_vy = 0;
    this->_animation_ctr = 0;
    this->_right = true;
    this->_alive = true;
}
void Axeman::draw(Camera camera, SDL_Renderer *renderer) const {
    if (!Axeman::DataWasLoaded) {
        Axeman::Textures = IMG_LoadTexture(renderer, "../data/images/characters/axeman.png");
        Axeman::Death = Mix_LoadWAV("../data/sounds/axe.mp3");
        Axeman::DataWasLoaded = true;
    }

    SDL_Rect screct = {64 * (int32_t)std::floor(this->_animation_ctr), 64 * !this->_right, 64, 64};
    SDL_Rect dstrect = {(int32_t)(this->_hitbox._x - 12 - camera.display()._x), (int32_t)(this->_hitbox._y - 4 - camera.display()._y), 64, 64};

    SDL_RenderCopy(renderer, Axeman::Textures, &screct, &dstrect);
}
void Axeman::update(Rect player_hitbox, std::list<Bullet> &bullets, const Map &map) {
    float dt = this->_timer();
    float dt_part;

    float air_factor1;

    for (int32_t i = 0; i <= (int32_t)(dt / (Axeman::MaximalUpdateDelay)); i = i + 1) {
        dt_part = Axeman::MaximalUpdateDelay;
        if (i == (int32_t)(dt / (Axeman::MaximalUpdateDelay))) dt_part = dt - (float)i * Axeman::MaximalUpdateDelay;

        air_factor1 = this->_get_air_factor(map, Axeman::air_factor);

        if (this->_possible_to_get_to_player(player_hitbox, map)) this->_right = (player_hitbox._x > this->_hitbox._x);
        this->_necessary_direction(this->_right, map);

        if (this->_right) this->_move_right(dt_part, air_factor1, Axeman::ax, Axeman::vx_max);
        else this->_move_left(dt_part, air_factor1, Axeman::ax, Axeman::vx_max);

        this->_gravity(dt_part, Axeman::ay);

        this->_update_x(dt_part, map);
        this->_update_y(dt_part, map);

        this->_animation_ctr = this->_animation_ctr + dt_part * 48;
        if (this->_animation_ctr >= 16) this->_animation_ctr = 0;
    }
}
void Axeman::kill() {
    if (this->_alive) {
        Mix_PlayChannel(-1, Axeman::Death, 0);
        this->disappear();
    }
}
bool Axeman::_possible_to_get_to_player(Rect player_hitbox, const Map &map) const {
    int32_t enemy_ty = (int32_t)(this->_hitbox._y + this->_hitbox._h - player_hitbox._h) / 32;
    int32_t player_ty = (int32_t)(player_hitbox._y) / 32;

    if (std::abs(this->_hitbox._x - player_hitbox._x) < 10000 and enemy_ty == player_ty) {
        int32_t from_tx = (int32_t)std::min(this->_hitbox._x, player_hitbox._x) / 32;
        int32_t to_tx = (int32_t)std::max(this->_hitbox._x, player_hitbox._x) / 32;

        for (int32_t tx = from_tx; tx < to_tx; tx = tx + 1) {
            if (map.solid(enemy_ty, tx) or map.dangerous(enemy_ty, tx) or (enemy_ty + 2 < map.ty() and map.dangerous(enemy_ty + 2, tx))) return false;
        }

        return true;
    }

    return false;
}