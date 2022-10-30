#include "LinkManager.hpp"


std::string LinkManager::path(const std::string& path) {
    return LinkManager::Path + '/' + path;
}
void LinkManager::set_directory(const std::string& path, bool in_home) {
    if (in_home) {
        LinkManager::Path = std::getenv("HOME");
        LinkManager::Path.push_back('/');
    }
    else LinkManager::Path.clear();

    LinkManager::Path.append(path);
}