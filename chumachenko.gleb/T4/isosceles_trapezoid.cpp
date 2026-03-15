#include "isosceles_trapezoid.h"
#include <stdexcept>

IsoscelesTrapezoid::IsoscelesTrapezoid(const Point& bottomLeft, double bBottom, double bTop, double height) :
    center_{ bottomLeft.x + bBottom / 2.0, bottomLeft.y + height / 2.0 },
    bBottom_(bBottom),
    bTop_(bTop),
    height_(height) {
    if (bBottom_ <= 0.0 || bTop_ <= 0.0 || height_ <= 0.0) {
        throw std::invalid_argument("Trapezoid parameters must be positive");
    }
}

double IsoscelesTrapezoid::getArea() const {
    return ((bBottom_ + bTop_) / 2.0) * height_;
}

Point IsoscelesTrapezoid::getCenter() const {
    return center_;
}

void IsoscelesTrapezoid::move(double dx, double dy) {
    center_.x += dx;
    center_.y += dy;
}

void IsoscelesTrapezoid::scale(double factor) {
    if (factor <= 0.0) {
        throw std::invalid_argument("Scale factor must be positive");
    }
    bBottom_ *= factor;
    bTop_ *= factor;
    height_ *= factor;
}

std::string IsoscelesTrapezoid::getName() const {
    return "ISOSCELES_TRAPEZOID";
}

std::unique_ptr<Shape> IsoscelesTrapezoid::clone() const {
    return std::make_unique<IsoscelesTrapezoid>(*this);
}


