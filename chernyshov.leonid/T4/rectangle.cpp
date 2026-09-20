#include "rectangle.h"
#include <stdexcept>

Rectangle::Rectangle(const Point& bottomLeft, const Point& topRight)
    : bottomLeft_(bottomLeft), topRight_(topRight) {
    if (topRight_.x <= bottomLeft_.x || topRight_.y <= bottomLeft_.y) {
        throw std::invalid_argument(
            "Rectangle: topRight must be above-right of bottomLeft");
    }
}

double Rectangle::getArea() const {
    return (topRight_.x - bottomLeft_.x) * (topRight_.y - bottomLeft_.y);
}

Point Rectangle::getCenter() const {
    return Point{(bottomLeft_.x + topRight_.x) / 2.0,
                 (bottomLeft_.y + topRight_.y) / 2.0};
}

void Rectangle::move(double dx, double dy) {
    bottomLeft_.x += dx;
    bottomLeft_.y += dy;
    topRight_.x += dx;
    topRight_.y += dy;
}

void Rectangle::scale(double factor) {
    if (factor <= 0.0) {
        throw std::invalid_argument("Scale factor must be positive");
    }

    Point center = getCenter();

    double halfW = (topRight_.x - bottomLeft_.x) / 2.0 * factor;
    double halfH = (topRight_.y - bottomLeft_.y) / 2.0 * factor;

    bottomLeft_ = Point{center.x - halfW, center.y - halfH};
    topRight_ = Point{center.x + halfW, center.y + halfH};
}

std::string Rectangle::getName() const {
    return "RECTANGLE";
}

std::unique_ptr<Shape> Rectangle::clone() const {
    return std::make_unique<Rectangle>(*this);
}
