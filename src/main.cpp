#include <filesystem>
#include "Player.hpp"
#include "Enemies.hpp"
#include "Background.hpp"
#include "Soundtrack.hpp"
#include "FPSCounter.hpp"


static SDL_Texture *Defeat;
static SDL_Texture *Finish;


void dark_screen(SDL_Renderer *renderer, Camera camera) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255 * 3 / 4);
    SDL_Rect rect = {0, 0, (int32_t)camera.display()._w, (int32_t)camera.display()._h};
    SDL_RenderFillRect(renderer, &rect);
}
void defeat(SDL_Renderer *renderer, Camera camera) {
    dark_screen(renderer, camera);

    SDL_Rect rect = {((int32_t)camera.display()._w - 6 * 80) / 2, ((int32_t)camera.display()._h - 80) / 2, 0, 0};
    SDL_QueryTexture(Defeat, nullptr, nullptr, &rect.w, &rect.h);
    SDL_RenderCopy(renderer, Defeat, nullptr, &rect);
}
void endgame(SDL_Renderer *renderer, Camera camera) {
    dark_screen(renderer, camera);

    SDL_Rect rect = {((int32_t)camera.display()._w - 4 * 80) / 2, ((int32_t)camera.display()._h - 80) / 2, 0, 0};
    SDL_QueryTexture(Finish, nullptr, nullptr, &rect.w, &rect.h);
    SDL_RenderCopy(renderer, Finish, nullptr, &rect);
}
int main() {
    LinkManager::set_directory("..", false);

    if (!std::filesystem::is_directory(LinkManager::path("appdata"))) std::filesystem::copy(LinkManager::path("data/appdata"), LinkManager::path("appdata"));

    SDL_Init(SDL_INIT_EVERYTHING);
    IMG_Init(IMG_INIT_PNG);
    Mix_Init(MIX_INIT_OGG | MIX_INIT_MP3);
    TTF_Init();

    SDL_DisplayMode dm;
    SDL_GetDesktopDisplayMode(0, &dm);

    SDL_Window *window = SDL_CreateWindow("Приключения Слизня", 0, 0, dm.w, dm.h, SDL_WINDOW_FULLSCREEN);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    SDL_SetRenderDrawBlendMode(renderer, static_cast<SDL_BlendMode>(SDL_TRUE));
    SDL_ShowCursor(0);
    SDL_SetWindowIcon(window, IMG_Load(LinkManager::path("data/images/logotype.png").c_str()));
    Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 2048);

    TTF_Font *font = TTF_OpenFont(LinkManager::path("data/fonts/BitterPro-Medium.ttf").c_str(), 80);
    SDL_Surface *surface = TTF_RenderUTF8_Blended(font, "Поражение", {255, 255, 255, 255});
    Defeat = SDL_CreateTextureFromSurface(renderer, surface);
    surface = TTF_RenderUTF8_Blended(font, "Конец", {255, 255, 255, 255});
    Finish = SDL_CreateTextureFromSurface(renderer, surface);
    TTF_CloseFont(font);

    SDL_Event event;
    bool left;
    bool right;
    bool jump;

    Map map = {LinkManager::path("data/level/map.tmx")};
    Camera camera = {{0, 0, 32 * (float)map.tx(), 32 * (float)map.ty()}, {0, 0, (float)dm.w, (float)dm.h}};
    Player player = {LinkManager::path("appdata/player.sles")};
    std::list<Bullet> bullets;
    Enemies main_enemies = {LinkManager::path("appdata/main_enemies.sles")};
    Enemies bridge_enemies = {LinkManager::path("appdata/bridge_enemies.sles")};
    Enemies river_enemies = {LinkManager::path("appdata/river_enemies.sles")};
    Background background;
    Soundtrack soundtrack;
    FPSCounter counter;
    bool show_FPS = false;

    bool endgame_was_activated = false;

    for (; ;) {
        counter.update();

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT or (event.type == SDL_KEYDOWN and event.key.keysym.sym == SDLK_ESCAPE)) return 0;
            else if (event.type == SDL_KEYDOWN and event.key.keysym.sym == SDLK_F10) show_FPS = !show_FPS;
            else if (event.type == SDL_KEYDOWN and event.key.keysym.sym == SDLK_RETURN) {
                player = {LinkManager::path("appdata/player.sles")};
                bullets.clear();
                main_enemies = {LinkManager::path("appdata/main_enemies.sles")};
                bridge_enemies = {LinkManager::path("appdata/bridge_enemies.sles")};
                river_enemies = {LinkManager::path("appdata/river_enemies.sles")};
                soundtrack.force_change();
            }
        }
        left = (SDL_GetKeyboardState(nullptr)[SDL_SCANCODE_A] or SDL_GetKeyboardState(nullptr)[SDL_SCANCODE_LEFT]) and !endgame_was_activated;
        right = (SDL_GetKeyboardState(nullptr)[SDL_SCANCODE_D] or SDL_GetKeyboardState(nullptr)[SDL_SCANCODE_RIGHT]) and !endgame_was_activated;
        jump = (SDL_GetKeyboardState(nullptr)[SDL_SCANCODE_W] or SDL_GetKeyboardState(nullptr)[SDL_SCANCODE_UP] or SDL_GetKeyboardState(nullptr)[SDL_SCANCODE_SPACE]) and !endgame_was_activated;

        camera.focus(player.hitbox()._x + player.hitbox()._w / 2, player.hitbox()._y + player.hitbox()._h / 2);
        if (player.alive()) player.update(left, right, jump, map);
        for (auto &bullet : bullets) {
            bullet.update(map);
            if (player.alive() and (player.hitbox() && bullet.hitbox())) player.kill();
        }
        for (auto &enemy : main_enemies._enemies) {
            enemy->update(player.hitbox(), bullets, map);
            if (player.alive() and (player.hitbox() && enemy->hitbox())) {
                if (player.hitbox()._y + player.hitbox()._h < enemy->hitbox()._y + enemy->hitbox()._h / 10) {
                    enemy->kill();
                    player.do_jump_after_kill();
                }
                else player.kill();
            }
        }
        bullets.erase(std::remove_if(bullets.begin(), bullets.end(), [](const Bullet& bullet){return !bullet.alive();}), bullets.end());
        main_enemies._enemies.erase(std::remove_if(main_enemies._enemies.begin(), main_enemies._enemies.end(), [](Enemy *enemy){return !enemy->alive();}), main_enemies._enemies.end());
        soundtrack.update();

        if (player.hitbox()._x >= 671 * 32) {
            main_enemies.add(bridge_enemies);
            bridge_enemies.clear();
        }
        if (player.hitbox()._x >= 1249 * 32) {
            main_enemies.add(river_enemies);
            river_enemies.clear();
        }
        if (player.hitbox()._x >= 1369 * 32) endgame_was_activated = true;
        if (player.alive() and player.in_house(map)) {
            player.save(LinkManager::path("appdata/player.sles"));
            main_enemies.save(LinkManager::path("appdata/main_enemies.sles"));
            bridge_enemies.save(LinkManager::path("appdata/bridge_enemies.sles"));
            river_enemies.save(LinkManager::path("appdata/river_enemies.sles"));
        }

        background.draw(camera, renderer);
        map.draw(camera, renderer);
        if (player.alive()) player.draw(camera, renderer);
        for (auto &enemy : main_enemies._enemies) enemy->draw(camera, renderer);
        for (auto &bullet : bullets) bullet.draw(camera, renderer);
        if (!player.alive()) defeat(renderer, camera);
        if (endgame_was_activated) endgame(renderer, camera);
        if (show_FPS) counter.draw(renderer);
        SDL_RenderPresent(renderer);
    }
}