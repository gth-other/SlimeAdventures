#include "Enemies.hpp"


Enemies::Enemies() = default;
Enemies::Enemies(const std::string& path) {
    std::ifstream file(path);

    std::stringstream ss;

    std::string buff;
    char type;
    float x, y;

    while (std::getline(file, buff)) {
        ss = std::stringstream(buff);
        ss >> type;
        if (ss.peek() == ' ') ss.ignore();
        ss >> x;
        if (ss.peek() == ' ') ss.ignore();
        ss >> y;
        if (ss.peek() == ' ') ss.ignore();

        switch (type) {
            case 's':
                this->_skeletons.emplace_back(x, y);
                this->_enemies.push_back(&this->_skeletons.back());
                break;
            case 'a':
                this->_axemen.emplace_back(x, y);
                this->_enemies.push_back(&this->_axemen.back());
                break;
            case 'g':
                this->_gunslingers.emplace_back(x, y);
                this->_enemies.push_back(&this->_gunslingers.back());
                break;
        }
    }

    file.close();
}
void Enemies::add(const Enemies& enemies) {
    for (const auto& s : enemies._skeletons) {
        this->_skeletons.push_back(s);
        this->_enemies.push_back(&this->_skeletons.back());
    }
    for (const auto& a : enemies._axemen) {
        this->_axemen.push_back(a);
        this->_enemies.push_back(&this->_axemen.back());
    }
    for (const auto& g : enemies._gunslingers) {
        this->_gunslingers.push_back(g);
        this->_enemies.push_back(&this->_gunslingers.back());
    }
}
void Enemies::save(const std::string& path) {
    std::ofstream file(path);

    for (const auto& s : this->_skeletons) if (s.alive()) file << 's' << ' ' << (int32_t)s.hitbox()._x << ' ' << (int32_t)s.hitbox()._y << std::endl;
    for (const auto& a : this->_axemen) if (a.alive()) file << 'a' << ' ' << (int32_t)a.hitbox()._x << ' ' << (int32_t)a.hitbox()._y << std::endl;
    for (const auto& g : this->_gunslingers) if (g.alive()) file << 'g' << ' ' << (int32_t)g.hitbox()._x << ' ' << (int32_t)g.hitbox()._y << std::endl;

    file.close();
}
void Enemies::clear() {
    this->_enemies.clear();

    this->_skeletons.clear();
    this->_axemen.clear();
    this->_gunslingers.clear();
}