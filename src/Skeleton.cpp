#include "Skeleton.hpp"


Skeleton::Skeleton() = default;
Skeleton::Skeleton(float x, float y) {
    this->_hitbox = {x, y, 30, 60};
    this->_t0 = 1e+9;
    this->_vx = 0;
    this->_vy = 0;
    this->_animation_ctr = 0;
    this->_right = true;
    this->_alive = true;
}
void Skeleton::draw(Camera camera, SDL_Renderer *renderer) const {
    if (!Skeleton::DataWasLoaded) {
        Skeleton::Textures = IMG_LoadTexture(renderer, "../data/images/characters/skeleton.png");
        Skeleton::Death = Mix_LoadWAV("../data/sounds/bones.mp3");
        Skeleton::DataWasLoaded = true;
    }

    SDL_Rect screct = {64 * (int32_t)std::floor(this->_animation_ctr), 64 * !this->_right, 64, 64};
    SDL_Rect dstrect = {(int32_t)(this->_hitbox._x - 17 - camera.display()._x), (int32_t)(this->_hitbox._y - 4 - camera.display()._y), 64, 64};

    SDL_RenderCopy(renderer, Skeleton::Textures, &screct, &dstrect);
}
void Skeleton::update(Rect player_hitbox, std::list<Bullet> &bullets, const Map &map) {
    float dt = this->_timer();
    float dt_part;

    float air_factor1;

    for (int32_t i = 0; i <= (int32_t)(dt / (1.f / 256.f)); i = i + 1) {
        dt_part = 1.f / 256.f;
        if (i == (int32_t)(dt / (1.f / 256.f))) dt_part = dt - (float)i * (1.f / 256.f);

        air_factor1 = this->_get_air_factor(map, Skeleton::air_factor);

        this->_necessary_direction(this->_right, map);

        if (this->_right) this->_move_right(dt_part, air_factor1, Skeleton::ax, Skeleton::vx_max);
        else this->_move_left(dt_part, air_factor1, Skeleton::ax, Skeleton::vx_max);

        this->_gravity(dt_part, Skeleton::ay);

        this->_update_x(dt_part, map);
        this->_update_y(dt_part, map);

        this->_animation_ctr = this->_animation_ctr + dt_part * 30;
        if (this->_animation_ctr >= 10) this->_animation_ctr = 0;
    }
}
void Skeleton::kill() {
    if (this->_alive) {
        Mix_PlayChannel(-1, Skeleton::Death, 0);
        this->disappear();
    }
}