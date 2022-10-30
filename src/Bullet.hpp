#include <SDL2/SDL_mixer.h>
#include "Character.hpp"


#pragma once


class Bullet : public Character {
public:
    Bullet();
    Bullet(float x, float y, float dst_x, float dst_y);

    void draw(Camera camera, SDL_Renderer *renderer) const override;
    void update(const Map &map);

    void kill() override;
private:
    static constexpr float v = 24 * 32;

    inline static SDL_Texture *Textures;
    inline static Mix_Chunk *Death;
    inline static bool DataWasLoaded = false;
};