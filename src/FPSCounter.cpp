#include "FPSCounter.hpp"


FPSCounter::FPSCounter() {
    this->_previous = 0;
    this->_updates = 0;
    this->_time = SDL_GetTicks();
}
void FPSCounter::update() {
    this->_updates = this->_updates + 1;
    if (SDL_GetTicks() - this->_time >= 1000) {
        this->_previous = this->_updates;
        this->_updates = 0;
        this->_time = SDL_GetTicks();
    }
}
void FPSCounter::draw(SDL_Renderer *renderer) {
    if (!FPSCounter::FontWasLoaded) {
        FPSCounter::Font = TTF_OpenFont(LinkManager::path("data/fonts/BitterPro-Medium.ttf").c_str(), 18);
        FPSCounter::FontWasLoaded = true;
    }

    SDL_Surface *surface = TTF_RenderUTF8_Blended(FPSCounter::Font, ("FPS: " + std::to_string(this->_previous)).c_str(), {255, 255, 255, 255});
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_Rect rect = {10, 10, 0, 0};
    SDL_QueryTexture(texture, nullptr, nullptr, &rect.w, &rect.h);
    SDL_RenderCopy(renderer, texture, nullptr, &rect);
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}