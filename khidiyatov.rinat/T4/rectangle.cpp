#include "rectangle.h"
#include <iostream>

Rectangle::Rectangle(const Point& bottomLeft, const Point& topRight) :
    bottomLeft_(bottomLeft),
    topRight_(topRight) {
}

double Rectangle::getArea() const {
    double width = topRight_.x_ - bottomLeft_.x_;
    double height = topRight_.y_ - bottomLeft_.y_;
    return width * height;
}

Point Rectangle::getCenter() const {
    double centerX = (bottomLeft_.x_ + topRight_.x_) / 2.0;
    double centerY = (bottomLeft_.y_ + topRight_.y_) / 2.0;
    return Point(centerX, centerY);
}

void Rectangle::move(double dx, double dy) {
    bottomLeft_.x_ += dx;
    bottomLeft_.y_ += dy;
    topRight_.x_ += dx;
    topRight_.y_ += dy;
}

void Rectangle::scale(double factor) {
    Point center = getCenter();
    double width = topRight_.x_ - bottomLeft_.x_;
    double height = topRight_.y_ - bottomLeft_.y_;

    double newWidth = width * factor;
    double newHeight = height * factor;

    bottomLeft_.x_ = center.x_ - newWidth / 2.0;
    bottomLeft_.y_ = center.y_ - newHeight / 2.0;
    topRight_.x_ = center.x_ + newWidth / 2.0;
    topRight_.y_ = center.y_ + newHeight / 2.0;
}

std::string Rectangle::getName() const {
    return "RECTANGLE";
}

void Rectangle::print(std::ostream& os, bool wrap) const {
    Point center = getCenter();
    double area = getArea();
    if (wrap) {
        os << "[";
    }
    os << getName() << ", (" << center.x_ << ", " << center.y_
        << "), " << area;
    if (wrap) {
        os << "]";
    }
}

std::pair<Point, Point> Rectangle::getBounds() const {
    return std::make_pair(bottomLeft_, topRight_);
}
