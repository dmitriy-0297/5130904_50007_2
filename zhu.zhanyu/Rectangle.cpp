#include "Rectangle.h"

Rectangle::Rectangle(Point leftBottom, Point rightTop)
    : leftBottom_(leftBottom), rightTop_(rightTop) {
}

double Rectangle::getArea() const {
    double width = rightTop_.x - leftBottom_.x;
    double height = rightTop_.y - leftBottom_.y;
    return width * height;
}

Point Rectangle::getCenter() const {
    Point Center;
    Center.x = (rightTop_.x + leftBottom_.x) / 2;
    Center.y = (rightTop_.y + leftBottom_.y) / 2;
    return Center;
}

void Rectangle::move(double dx, double dy) {
    rightTop_.x += dx;
    rightTop_.y += dy;
    leftBottom_.x += dx;
    leftBottom_.y += dy;
}

void Rectangle::scale(double factor) {
    Point center = getCenter();
    double width = rightTop_.x - leftBottom_.x;
    double height = rightTop_.y - leftBottom_.y;
    double newWidth = width * factor;
    double newHeight = height * factor;
    leftBottom_.x = center.x - newWidth / 2;
    leftBottom_.y = center.y - newHeight / 2;
    rightTop_.x = center.x + newWidth / 2;
    rightTop_.y = center.y + newHeight / 2;
}

std::string Rectangle::getName() const {
    return "RECTANGLE";
}

double Rectangle::getMinX() const { return leftBottom_.x; }
double Rectangle::getMaxX() const { return rightTop_.x; }
double Rectangle::getMinY() const { return leftBottom_.y; }
double Rectangle::getMaxY() const { return rightTop_.y; }
