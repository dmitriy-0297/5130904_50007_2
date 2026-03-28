#include "circle.h"
#include <iostream>
#include <cmath>

constexpr double PI = 3.141592653589793;

Circle::Circle(const Point& center, double radius) :
    center_(center),
    radius_(radius) {
}

double Circle::getArea() const {
    return PI * radius_ * radius_;
}

Point Circle::getCenter() const {
    return center_;
}

void Circle::move(double dx, double dy) {
    center_.x_ += dx;
    center_.y_ += dy;
}

void Circle::scale(double factor) {
    radius_ *= factor;
}

std::string Circle::getName() const {
    return "CIRCLE";
}

void Circle::print(std::ostream& os, bool wrap) const {
    double area = getArea();
    if (wrap) {
        os << "[";
    }
    os << getName() << ", (" << center_.x_ << ", " << center_.y_
        << "), " << area;
    if (wrap) {
        os << "]";
    }
}

std::pair<Point, Point> Circle::getBounds() const {
    Point min(center_.x_ - radius_, center_.y_ - radius_);
    Point max(center_.x_ + radius_, center_.y_ + radius_);
    return std::make_pair(min, max);
}
