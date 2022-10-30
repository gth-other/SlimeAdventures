#include <string>


#pragma once


class LinkManager {
public:
    static std::string path(const std::string& path);
    static void set_directory(const std::string& path, bool in_home);
private:
    inline static std::string Path;
};