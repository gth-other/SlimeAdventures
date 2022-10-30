#include "Rect.hpp"


Rect::Rect() = default;
Rect::Rect(float x, float y, float w, float h) {
    this->_x = x;
    this->_y = y;
    this->_w = w;
    this->_h = h;
}
bool operator &&(Rect a, Rect b) {
    return std::max(a._x, b._x) < std::min(a._x + a._w, b._x + b._w) and std::max(a._y, b._y) < std::min(a._y + a._h, b._y + b._h);
}