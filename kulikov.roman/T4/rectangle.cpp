#include "rectangle.h"
#include <iostream>
#include <iomanip>

Rectangle::Rectangle(Point bl, Point tr)
    : lowLeft_(bl)
    , topRight_(tr)
{
}

double Rectangle::getArea() const
{
    const double width = topRight_.x - lowLeft_.x;
    const double height = topRight_.y - lowLeft_.y;
    return width * height;
}

Point Rectangle::getCenter() const
{
    const double centerX = (lowLeft_.x + topRight_.x) / 2.0;
    const double centerY = (lowLeft_.y + topRight_.y) / 2.0;
    return Point(centerX, centerY);
}

void Rectangle::move(double dx, double dy)
{
    lowLeft_.x += dx;
    lowLeft_.y += dy;
    topRight_.x += dx;
    topRight_.y += dy;
}

void Rectangle::scale(double k)
{
    const Point center = getCenter();
    const double width = topRight_.x - lowLeft_.x;
    const double height = topRight_.y - lowLeft_.y;

    const double newWidth = width * k;
    const double newHeight = height * k;

    lowLeft_.x = center.x - newWidth / 2.0;
    lowLeft_.y = center.y - newHeight / 2.0;
    topRight_.x = center.x + newWidth / 2.0;
    topRight_.y = center.y + newHeight / 2.0;
}

const char* Rectangle::getName() const
{
    return "RECTANGLE";
}

void Rectangle::print() const
{
    std::cout << std::fixed << std::setprecision(2);
    std::cout << "[" << getName() << ", ("
              << getCenter().x << ", "
              << getCenter().y << "), "
              << getArea() << "]";
}

Point Rectangle::getBottomLeft() const
{
    return lowLeft_;
}

Point Rectangle::getTopRight() const
{
    return topRight_;
}
