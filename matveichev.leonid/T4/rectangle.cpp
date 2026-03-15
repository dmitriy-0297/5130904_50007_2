#include "rectangle.h"
#include <iomanip>
#include <algorithm>

Rectangle::Rectangle(const Point& bottomLeft, const Point& topRight)
    : bottomLeft_(bottomLeft)
    , topRight_(topRight)
{}

double Rectangle::getArea() const {
    double width = topRight_.x_ - bottomLeft_.x_;
    double height = topRight_.y_ - bottomLeft_.y_;
    if (width < 0) {
        width = -width;
    }
    if (height < 0) {
        height = -height;
    }
    return width * height;
}

Point Rectangle::getCenter() const {
    return Point((bottomLeft_.x_ + topRight_.x_) / 2.0,
                 (bottomLeft_.y_ + topRight_.y_) / 2.0);
}

void Rectangle::move(double dx, double dy) {
    bottomLeft_.x_ += dx;
    bottomLeft_.y_ += dy;
    topRight_.x_ += dx;
    topRight_.y_ += dy;
}

void Rectangle::scale(double coefficient) {
    Point center = getCenter();
    double width = topRight_.x_ - bottomLeft_.x_;
    double height = topRight_.y_ - bottomLeft_.y_;

    double newWidth = width * coefficient;
    double newHeight = height * coefficient;

    bottomLeft_.x_ = center.x_ - newWidth / 2.0;
    bottomLeft_.y_ = center.y_ - newHeight / 2.0;
    topRight_.x_ = center.x_ + newWidth / 2.0;
    topRight_.y_ = center.y_ + newHeight / 2.0;
}

std::string Rectangle::getName() const {
    return "RECTANGLE";
}

void Rectangle::print(std::ostream& os) const {
    os << std::fixed << std::setprecision(2);
    os << "[" << getName() << ", ("
       << getCenter().x_ << ", " << getCenter().y_
       << "), " << getArea() << "]";
}

void Rectangle::getBoundingBox(Point& min, Point& max) const {
    min = Point(std::min(bottomLeft_.x_, topRight_.x_),
                std::min(bottomLeft_.y_, topRight_.y_));
    max = Point(std::max(bottomLeft_.x_, topRight_.x_),
                std::max(bottomLeft_.y_, topRight_.y_));
}
