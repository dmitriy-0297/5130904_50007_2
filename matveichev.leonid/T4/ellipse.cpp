#include "ellipse.h"
#include <iomanip>
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

Ellipse::Ellipse(const Point& center, double radiusX, double radiusY)
    : center_(center), radiusX_(radiusX), radiusY_(radiusY) {}

double Ellipse::getArea() const {
    return M_PI * radiusX_ * radiusY_;
}

Point Ellipse::getCenter() const {
    return center_;
}

void Ellipse::move(double dx, double dy) {
    center_.x += dx;
    center_.y += dy;
}

void Ellipse::scale(double coefficient) {
    radiusX_ *= coefficient;
    radiusY_ *= coefficient;
}

std::string Ellipse::getName() const {
    return "ELLIPSE";
}

void Ellipse::print(std::ostream& os) const {
    os << std::fixed << std::setprecision(2);
    os << "[" << getName() << ", (" << center_.x << ", " << center_.y << "), " << getArea() << "]";
}

void Ellipse::getBoundingBox(Point& min, Point& max) const {
    min = Point(center_.x - radiusX_, center_.y - radiusY_);
    max = Point(center_.x + radiusX_, center_.y + radiusY_);
}
