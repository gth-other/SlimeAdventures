#include "Player.hpp"


Player::Player() = default;
Player::Player(float x, float y) {
    this->_hitbox = {x, y, 28, 20};
    this->_t0 = 1e+9;
    this->_vx = 0;
    this->_vy = 0;
    this->_animation_ctr = 0;
    this->_right = true;
    this->_alive = true;
}
Player::Player(const std::string& path) {
    std::ifstream file(path);

    std::string buff0, buff1;
    std::getline(file, buff0);
    std::getline(file, buff1);

    file.close();

    *this = {std::stof(buff0), std::stof(buff1)};
}
void Player::draw(Camera camera, SDL_Renderer *renderer) const {
    if (!Player::DataWasLoaded) {
        Player::Textures = IMG_LoadTexture(renderer, LinkManager::path("data/images/characters/slime.png").c_str());
        Player::Death = Mix_LoadWAV(LinkManager::path("data/sounds/water.mp3").c_str());
        Player::DataWasLoaded = true;
    }

    SDL_Rect screct = {32 * (int32_t)std::floor(this->_animation_ctr), 32 * !this->_right, 32, 32};
    SDL_Rect dstrect = {(int32_t)(this->_hitbox._x - 4 - camera.display()._x), (int32_t)(this->_hitbox._y - 12 - camera.display()._y), 32, 32};

    SDL_RenderCopy(renderer, Player::Textures, &screct, &dstrect);
}
void Player::update(bool left, bool right, bool jump, const Map &map) {
    float dt = this->_timer();
    float dt_part;

    float air_factor1;

    for (int32_t i = 0; i <= (int32_t)(dt / Player::MaximalUpdateDelay); i = i + 1) {
        dt_part = Player::MaximalUpdateDelay;
        if (i == (int32_t)(dt / Player::MaximalUpdateDelay)) dt_part = dt - (float)i * Player::MaximalUpdateDelay;

        air_factor1 = this->_get_air_factor(map, Player::air_factor);

        if (left == right) this->_stop(dt_part, air_factor1, Player::ax);
        else if (left) this->_move_left(dt_part, air_factor1, Player::ax, Player::vx_max);
        else if (right) this->_move_right(dt_part, air_factor1, Player::ax, Player::vx_max);
        if (jump) this->_jump(Player::vy_jump, map);

        this->_gravity(dt_part, Player::ay);

        this->_update_x(dt_part, map);
        this->_update_y(dt_part, map);

        this->_animation_ctr = this->_animation_ctr + dt_part * 8;
        if (this->_animation_ctr >= 4) this->_animation_ctr = 0;
    }
}
void Player::save(const std::string& path) {
    std::ofstream file(path);
    file << (int32_t)this->_hitbox._x << '\n' << (int32_t)this->_hitbox._y;
    file.close();
}
void Player::do_jump_after_kill() {
    this->_vy = Player::vy_kill;
}
void Player::kill() {
    if (this->_alive) {
        Mix_PlayChannel(-1, Player::Death, 0);
        this->_alive = false;
    }
}
bool Player::in_house(const Map &map) const {
    return map.house(std::max(0, (int32_t)this->_hitbox._y / 32 - 1), (int32_t)this->_hitbox._x / 32);
}