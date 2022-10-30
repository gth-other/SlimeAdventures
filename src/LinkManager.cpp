#include "LinkManager.hpp"


std::string LinkManager::path(const std::string& path, bool resource) {
    if (resource) return LinkManager::ResourcePath + '/' + path;
    return LinkManager::UserPath + '/' + path;
}
void LinkManager::set_directory(std::string resource_path, std::string user_path) {
    LinkManager::ResourcePath = std::move(resource_path);
    LinkManager::UserPath = std::move(user_path);
}