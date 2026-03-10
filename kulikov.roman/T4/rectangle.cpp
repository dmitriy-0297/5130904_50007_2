#include "rectangle.h"
#include <iostream>
#include <iomanip>

Rectangle::Rectangle(Point bl, Point tr) : Left(bl), topRight(tr) {}

double Rectangle::getArea() const {
    double width = topRight.x - Left.x;
    double height = topRight.y - Left.y;
    return width * height;
}

Point Rectangle::getCenter() const {
    double centerX = (Left.x + topRight.x) / 2.0;
    double centerY = (Left.y + topRight.y) / 2.0;
    return Point(centerX, centerY);
}

void Rectangle::move(double dx, double dy) {
    Left.x += dx;
    Left.y += dy;
    topRight.x += dx;
    topRight.y += dy;
}

void Rectangle::scale(double coefficient) {
    Point center = getCenter();
    double width = topRight.x - Left.x;
    double height = topRight.y - Left.y;

    width *= coefficient;
    height *= coefficient;

    Left.x = center.x - width / 2.0;
    Left.y = center.y - height / 2.0;
    topRight.x = center.x + width / 2.0;
    topRight.y = center.y + height / 2.0;
}

const char* Rectangle::getName() const {
    return "RECTANGLE";
}

void Rectangle::print() const {
    std::cout << std::fixed << std::setprecision(2);
    std::cout << "[" << getName() << ", (" << getCenter().x << ", "
              << getCenter().y << "), " << getArea() << "]";
}
