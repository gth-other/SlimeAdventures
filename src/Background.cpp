#include "Background.hpp"


Background::Background() = default;
void Background::draw(Camera camera, SDL_Renderer *renderer) const {
    if (!Background::TextureWasLoaded) {
        Background::Texture = IMG_LoadTexture(renderer, LinkManager::path("data/images/background.png").c_str());
        Background::TextureWasLoaded = true;
    }

    SDL_SetRenderDrawColor(renderer, 89, 110, 132, 255);
    SDL_RenderClear(renderer);

    SDL_Rect rect;
    for (int32_t x = -(int32_t)camera.display()._x / 4 % 928; x < (int32_t)camera.display()._w; x = x + 928) {
        rect = {x, (int32_t)camera.display()._h - 793, 928, 793};
        SDL_RenderCopy(renderer, Background::Texture, nullptr, &rect);
    }
}