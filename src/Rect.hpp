#include <algorithm>


#pragma once


// Здесь и далее не используется декартовая система координат!
struct Rect {
    Rect();
    Rect(float x, float y, float w, float h);

    friend bool operator &&(Rect l, Rect r);

    float _x;
    float _y;
    float _w;
    float _h;
};