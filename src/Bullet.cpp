#include "Bullet.hpp"


Bullet::Bullet() = default;
Bullet::Bullet(float x, float y, float dst_x, float dst_y) {
    this->_hitbox = {x, y, 1, 1};
    this->_t0 = 1e+9;
    this->_vy = (float)std::sqrt(std::pow(dst_y * Bullet::v, 2) / (std::pow(dst_y, 2) + std::pow(dst_x, 2)));
    this->_vx = (float)std::sqrt(std::pow(Bullet::v, 2) - std::pow(this->_vy, 2));
    this->_alive = true;

    if (dst_x < 0) this->_vx = -this->_vx;
    if (dst_y < 0) this->_vy = -this->_vy;
}
void Bullet::draw(Camera camera, SDL_Renderer *renderer) const {
    if (!Bullet::DataWasLoaded) {
        Bullet::Textures = IMG_LoadTexture(renderer, "../data/images/characters/bullet.png");
        Bullet::Death = Mix_LoadWAV("../data/sounds/ricochet.mp3");
        Bullet::DataWasLoaded = true;
    }

    SDL_Rect screct = {0, 10 * (this->_vx < 0), 10, 10};
    SDL_Rect dstrect = {(int32_t)(this->_hitbox._x - camera.display()._x), (int32_t)(this->_hitbox._y - camera.display()._y), 10, 10};
    SDL_RenderCopy(renderer, Bullet::Textures, &screct, &dstrect);
}
void Bullet::update(const Map &map) {
    float dt = this->_timer();
    float dt_part;

    for (int32_t i = 0; i <= (int32_t)(dt / Bullet::MaximalUpdateDelay); i = i + 1) {
        dt_part = Bullet::MaximalUpdateDelay;
        if (i == (int32_t)(dt / Bullet::MaximalUpdateDelay)) dt_part = dt - (float)i * Bullet::MaximalUpdateDelay;

        this->_update_x(dt_part, map);
        this->_update_y(dt_part, map);

        if (this->_vx == 0 or this->_vy == 0) this->kill();
    }
}
void Bullet::kill() {
    if (this->_alive) {
        Mix_PlayChannel(-1, Bullet::Death, 0);
        this->disappear();
    }
}