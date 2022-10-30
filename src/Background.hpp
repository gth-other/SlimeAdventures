#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <cstdint>
#include "Camera.hpp"


#pragma once


class Background {
public:
    Background();

    void draw(Camera camera, SDL_Renderer *renderer) const;
private:
    inline static SDL_Texture *Texture;
    inline static bool TextureWasLoaded = false;
};