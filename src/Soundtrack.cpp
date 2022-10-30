#include "Soundtrack.hpp"


Soundtrack::Soundtrack() {
    this->_last_music = (int32_t)time(nullptr) % 2;
    Mix_PlayMusic(Soundtrack::Music[this->_last_music], 0);
}
void Soundtrack::update() {
    if (!Soundtrack::MusicWasLoaded) {
        Soundtrack::Music[0] = Mix_LoadMUS("../data/music/1.ogg");
        Soundtrack::Music[1] = Mix_LoadMUS("../data/music/2.ogg");
        Soundtrack::MusicWasLoaded = true;
    }

    if (!Mix_PlayingMusic()) this->force_change();
}
void Soundtrack::force_change() {
    this->_last_music = (this->_last_music + 1) % 2;
    Mix_PlayMusic(Soundtrack::Music[this->_last_music], 0);
}