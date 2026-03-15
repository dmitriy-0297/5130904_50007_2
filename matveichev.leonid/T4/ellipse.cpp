#include "ellipse.h"
#include <iomanip>
#include <cmath>

constexpr double PI = 3.14159265358979323846;

Ellipse::Ellipse(const Point& center, double radiusX, double radiusY)
    : center_(center)
    , radiusX_(radiusX)
    , radiusY_(radiusY)
{}

double Ellipse::getArea() const {
    return PI * radiusX_ * radiusY_;
}

Point Ellipse::getCenter() const {
    return center_;
}

void Ellipse::move(double dx, double dy) {
    center_.x_ += dx;
    center_.y_ += dy;
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
    os << "[" << getName() << ", ("
       << center_.x_ << ", " << center_.y_
       << "), " << getArea() << "]";
}

void Ellipse::getBoundingBox(Point& min, Point& max) const {
    min = Point(center_.x_ - radiusX_, center_.y_ - radiusY_);
    max = Point(center_.x_ + radiusX_, center_.y_ + radiusY_);
}
