#include <vector>
#include <fstream>
#include <cstdint>
#include <sstream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "Camera.hpp"


#pragma once


class Map {
public:
    Map();
    Map(const std::string& path);

    void draw(Camera camera, SDL_Renderer *renderer) const;

    [[nodiscard]] bool solid(int32_t ty, int32_t tx) const;
    [[nodiscard]] bool dangerous(int32_t ty, int32_t tx) const;
    [[nodiscard]] bool house(int32_t ty, int32_t tx) const;

    [[nodiscard]] int32_t tx() const;
    [[nodiscard]] int32_t ty() const;
private:
    std::vector<std::vector<uint16_t>> _tiles;

    struct Textures {
        inline static SDL_Texture *SolidTiles;
        inline static SDL_Texture *DangerousTiles;
        inline static SDL_Texture *DecorationTiles;
        inline static bool TexturesWereLoaded = false;
    };
    struct Borders {
        inline static std::pair<uint16_t, uint16_t> SolidTiles = {1, 128};
        inline static std::pair<uint16_t, uint16_t> DangerousTiles = {329, 338};
        inline static std::pair<uint16_t, uint16_t> DecorationTiles = {129, 328};
        inline static std::pair<uint16_t, uint16_t> AnimatedWater = {334, 337};
    };
};