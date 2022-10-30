#include "Rect.hpp"


#pragma once


class Camera {
public:
    Camera();
    Camera(Rect level, Rect display);

    void focus(float x, float y);
    [[nodiscard]] Rect display() const;
private:
    Rect _level;
    Rect _display;
};