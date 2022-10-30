#include <SDL2/SDL_mixer.h>
#include <array>
#include <cstdint>
#include <ctime>
#include "LinkManager.hpp"


#pragma once


class Soundtrack {
public:
    Soundtrack();

    void update();
    void force_change();
private:
    int32_t _last_music;

    inline static std::array<Mix_Music*, 2> Music;
    inline static bool MusicWasLoaded = false;
};