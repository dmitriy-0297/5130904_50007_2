#include "rectangle.h"
#include <stdexcept>
#include <cmath>

Rectangle::Rectangle(const Point& bottomLeft, const Point& topRight) :
    center_{ (bottomLeft.x + topRight.x) / 2.0, (bottomLeft.y + topRight.y) / 2.0 },
    width_(std::abs(topRight.x - bottomLeft.x)),
    height_(std::abs(topRight.y - bottomLeft.y)) {
    if (width_ <= 0.0 || height_ <= 0.0) {
        throw std::invalid_argument("Rectangle dimensions must be positive");
    }
}

double Rectangle::getArea() const {
    return width_ * height_;
}

Point Rectangle::getCenter() const {
    return center_;
}

void Rectangle::move(double dx, double dy) {
    center_.x += dx;
    center_.y += dy;
}

void Rectangle::scale(double factor) {
    if (factor <= 0.0) {
        throw std::invalid_argument("Scale factor must be positive");
    }
    width_ *= factor;
    height_ *= factor;
}

std::string Rectangle::getName() const {
    return "RECTANGLE";
}

std::unique_ptr<Shape> Rectangle::clone() const {
    return std::make_unique<Rectangle>(*this);
}

