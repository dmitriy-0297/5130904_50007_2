#include "Rectangle.h"

Rectangle::Rectangle(Point leftBottom, Point rightTop)
    : leftBottom(leftBottom), rightTop(rightTop)
{
}

double Rectangle::getArea() const
{
    return (rightTop.x - leftBottom.x) *
        (rightTop.y - leftBottom.y);
}

Point Rectangle::getCenter() const
{
    return {
        (leftBottom.x + rightTop.x) / 2.0,
        (leftBottom.y + rightTop.y) / 2.0
    };
}

void Rectangle::move(double dx, double dy)
{
    leftBottom.x += dx;
    leftBottom.y += dy;

    rightTop.x += dx;
    rightTop.y += dy;
}

void Rectangle::scale(double factor)
{
    Point center = getCenter();

    leftBottom.x = center.x + (leftBottom.x - center.x) * factor;
    leftBottom.y = center.y + (leftBottom.y - center.y) * factor;

    rightTop.x = center.x + (rightTop.x - center.x) * factor;
    rightTop.y = center.y + (rightTop.y - center.y) * factor;
}

const char* Rectangle::getName() const
{
    return "RECTANGLE";
}

double Rectangle::getMinX() const
{
    return leftBottom.x;
}

double Rectangle::getMaxX() const
{
    return rightTop.x;
}

double Rectangle::getMinY() const
{
    return leftBottom.y;
}

double Rectangle::getMaxY() const
{
    return rightTop.y;
}