#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <cstdint>
#include <string>


#pragma once


class FPSCounter {
public:
    FPSCounter();

    void update();

    void draw(SDL_Renderer *renderer);
private:
    int32_t _previous;
    int32_t _updates;
    uint32_t _time;

    TTF_Font *Font;
    inline static bool FontWasLoaded = false;
};