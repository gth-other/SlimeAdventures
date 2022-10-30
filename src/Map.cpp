#include "Map.hpp"


Map::Map() = default;
Map::Map(const std::string& path) {
    // Проверено только с файлами, созданными в Tiled 1.4.3 (Linux).

    std::ifstream file(path);

    uint16_t number;
    std::string line;
    std::stringstream ss;

    do std::getline(file, line);
    while (line != "  <data encoding=\"csv\">");

    for (; ;) {
        std::getline(file, line);
        if (line == "</data>") break;

        this->_tiles.resize(this->_tiles.size() + 1);
        ss = std::stringstream(line);
        while (ss >> number) {
            this->_tiles.back().push_back(number);
            if (ss.peek() == ',') ss.ignore();
        }
    }

    file.close();
}
void Map::draw(Camera camera, SDL_Renderer *renderer) const {
    if (!Map::Textures::TexturesWereLoaded) {
        Map::Textures::SolidTiles = IMG_LoadTexture(renderer, LinkManager::path("data/images/tiles/solid.png").c_str());
        Map::Textures::DangerousTiles = IMG_LoadTexture(renderer, LinkManager::path("data/images/tiles/dangerous.png").c_str());
        Map::Textures::DecorationTiles = IMG_LoadTexture(renderer, LinkManager::path("data/images/tiles/decoration.png").c_str());
        Map::Textures::TexturesWereLoaded = true;
    }

    uint16_t code;
    SDL_Rect screct, dstrect;

    for (int32_t ty = (int32_t)camera.display()._y / 32; ty <= std::min(this->ty() - 1, ((int32_t)camera.display()._y + (int32_t)camera.display()._h) / 32); ty = ty + 1) {
        for (int32_t tx = (int32_t)camera.display()._x / 32; tx <= std::min(this->tx() - 1, ((int32_t)camera.display()._x + (int32_t)camera.display()._w) / 32); tx = tx + 1) {
            code = this->_tiles[ty][tx];

            if (code >= Map::Borders::AnimatedWater.first and code <= Map::Borders::AnimatedWater.second) {
                code = Map::Borders::AnimatedWater.first + (SDL_GetTicks() / 100) % (Map::Borders::AnimatedWater.second - Map::Borders::AnimatedWater.first + 1);
            }

            if (code >= Map::Borders::SolidTiles.first and code <= Map::Borders::SolidTiles.second) {
                screct = {32 * (code - Map::Borders::SolidTiles.first) % 512, 32 * (code - Map::Borders::SolidTiles.first) / 512 * 32, 32, 32};
                dstrect = {32 * tx - (int32_t)camera.display()._x, 32 * ty - (int32_t)camera.display()._y, 32, 32};
                SDL_RenderCopy(renderer, Map::Textures::SolidTiles, &screct, &dstrect);
            }
            else if (code >= Map::Borders::DangerousTiles.first and code <= Map::Borders::DangerousTiles.second) {
                screct = {32 * (code - Map::Borders::DangerousTiles.first) % 160, 32 * (code - Map::Borders::DangerousTiles.first) / 160 * 32, 32, 32};
                dstrect = {32 * tx - (int32_t)camera.display()._x, 32 * ty - (int32_t)camera.display()._y, 32, 32};
                SDL_RenderCopy(renderer, Map::Textures::DangerousTiles, &screct, &dstrect);
            }
            else if (code >= Map::Borders::DecorationTiles.first and code <= Map::Borders::DecorationTiles.second) {
                screct = {32 * (code - Map::Borders::DecorationTiles.first) % 640, 32 * (code - Map::Borders::DecorationTiles.first) / 640 * 32, 32, 32};
                dstrect = {32 * tx - (int32_t)camera.display()._x, 32 * ty - (int32_t)camera.display()._y, 32, 32};
                SDL_RenderCopy(renderer, Map::Textures::DecorationTiles, &screct, &dstrect);
            }
        }
    }
}
bool Map::solid(int32_t ty, int32_t tx) const {
    uint16_t code = this->_tiles[ty][tx];
    return (code >= Map::Borders::SolidTiles.first and code <= Map::Borders::SolidTiles.second);
}
bool Map::dangerous(int32_t ty, int32_t tx) const {
    uint16_t code = this->_tiles[ty][tx];
    return (code >= Map::Borders::DangerousTiles.first and code <= Map::Borders::DangerousTiles.second);
}
bool Map::house(int32_t ty, int32_t tx) const {
    uint16_t code = this->_tiles[ty][tx];
    if (code < Map::Borders::DecorationTiles.first or code > Map::Borders::DecorationTiles.second) return false;

    code = code - Map::Borders::DecorationTiles.first;

    uint16_t x = code % 20;
    uint16_t y = code / 20;

    return (x >= 7 and y >= 4 and x <= 14 and y <= 8);
}
int32_t Map::tx() const {
    return (int32_t)this->_tiles[0].size();
}
int32_t Map::ty() const {
    return (int32_t)this->_tiles.size();
}