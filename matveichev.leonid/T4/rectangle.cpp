#include "rectangle.h"
#include <iomanip>
#include <algorithm>

Rectangle::Rectangle(const Point& bottomLeft, const Point& topRight)
    : bottomLeft_(bottomLeft), topRight_(topRight) {}

double Rectangle::getArea() const {
    double width = topRight_.x - bottomLeft_.x;
    double height = topRight_.y - bottomLeft_.y;
    if (width < 0) width = -width;
    if (height < 0) height = -height;
    return width * height;
}

Point Rectangle::getCenter() const {
    return Point((bottomLeft_.x + topRight_.x) / 2.0,
                 (bottomLeft_.y + topRight_.y) / 2.0);
}

void Rectangle::move(double dx, double dy) {
    bottomLeft_.x += dx;
    bottomLeft_.y += dy;
    topRight_.x += dx;
    topRight_.y += dy;
}

void Rectangle::scale(double coefficient) {
    Point center = getCenter();
    double width = topRight_.x - bottomLeft_.x;
    double height = topRight_.y - bottomLeft_.y;

    double newWidth = width * coefficient;
    double newHeight = height * coefficient;

    bottomLeft_.x = center.x - newWidth / 2.0;
    bottomLeft_.y = center.y - newHeight / 2.0;
    topRight_.x = center.x + newWidth / 2.0;
    topRight_.y = center.y + newHeight / 2.0;
}

std::string Rectangle::getName() const {
    return "RECTANGLE";
}

void Rectangle::print(std::ostream& os) const {
    os << std::fixed << std::setprecision(2);
    os << "[" << getName() << ", (" << getCenter().x << ", " << getCenter().y << "), " << getArea() << "]";
}

void Rectangle::getBoundingBox(Point& min, Point& max) const {
    min = Point(std::min(bottomLeft_.x, topRight_.x), std::min(bottomLeft_.y, topRight_.y));
    max = Point(std::max(bottomLeft_.x, topRight_.x), std::max(bottomLeft_.y, topRight_.y));
}
