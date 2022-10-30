cmake_minimum_required(VERSION 3.22)
project(SlimeAdventures)
set(CMAKE_CXX_STANDARD 23)
add_executable(SlimeAdventures
        ../src/Rect.hpp ../src/Rect.cpp
        ../src/Camera.hpp ../src/Camera.cpp
        ../src/Map.hpp ../src/Map.cpp
        ../src/Character.hpp ../src/Character.cpp
        ../src/Player.hpp ../src/Player.cpp
        ../src/Enemy.hpp ../src/Enemy.cpp
        ../src/Skeleton.hpp ../src/Skeleton.cpp
        ../src/Axeman.hpp ../src/Axeman.cpp
        ../src/Bullet.hpp ../src/Bullet.cpp
        ../src/Gunslinger.hpp ../src/Gunslinger.cpp
        ../src/Background.hpp ../src/Background.cpp
        ../src/Soundtrack.hpp ../src/Soundtrack.cpp
        ../src/FPSCounter.hpp ../src/FPSCounter.cpp
        ../src/Enemies.hpp ../src/Enemies.cpp
        ../src/main.cpp)
include(FindPkgConfig)
add_definitions(-Werror)
add_definitions(-O3)
add_definitions(-flto)
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)


# Подключение SDL2 и дополнений.
find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_mixer REQUIRED)
find_package(SDL2_ttf REQUIRED)
include_directories(${SDL2_INCLUDE_DIR} ${SDL2_IMAGE_INCLUDE_DIR} ${SDL2_MIXER_INCLUDE_DIR} ${SDL2_TTF_INCLUDE_DIR})
target_link_libraries(SlimeAdventures ${SDL2_LIBRARY} ${SDL2_IMAGE_LIBRARIES} ${SDL2_MIXER_LIBRARIES} ${SDL2_TTF_LIBRARIES})