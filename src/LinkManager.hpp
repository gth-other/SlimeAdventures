#include <string>
#include <utility>


#pragma once


class LinkManager {
public:
    static std::string path(const std::string& path, bool resource);
    static void set_directory(std::string resource_path, std::string user_path);
private:
    inline static std::string ResourcePath;
    inline static std::string UserPath;
};