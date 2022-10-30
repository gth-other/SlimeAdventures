#include "Camera.hpp"


Camera::Camera() = default;
Camera::Camera(Rect level, Rect display) {
    this->_level = level;
    this->_display = display;
}
void Camera::focus(float x, float y) {
    this->_display._x = std::min(this->_level._w - this->_display._w, std::max(0.f, x - this->_display._w / 2));
    this->_display._y = std::min(this->_level._h - this->_display._h, std::max(0.f, y - this->_display._h / 2));
}
Rect Camera::display() const {
    return this->_display;
}