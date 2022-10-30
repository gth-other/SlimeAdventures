#include <SDL_mixer.h>
#include "Character.hpp"


#pragma once


class Player : public Character {
public:
    Player();
    Player(float x, float y);
    Player(const std::string& path);

    void draw(Camera camera, SDL_Renderer *renderer) const override;
    void update(bool left, bool right, bool jump, const Map &map);

    void save(const std::string& path);

    void do_jump_after_kill();
    void kill() override;

    [[nodiscard]] bool in_house(const Map &map) const;
private:
    static constexpr float ax = 24 * 32;
    static constexpr float vx_max = 6 * 32;
    static constexpr float air_factor = 0.25;

    static constexpr float ay = 10 * 32;
    static constexpr float vy_jump = -8 * 32;
    static constexpr float vy_kill = -6 * 32;

    inline static SDL_Texture *Textures;
    inline static Mix_Chunk *Death;
    inline static bool DataWasLoaded = false;
};