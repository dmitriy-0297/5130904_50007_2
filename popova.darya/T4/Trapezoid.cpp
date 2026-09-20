#include "Trapezoid.h"

#include <algorithm>


Trapezoid::Trapezoid(Point leftBottom,
    double bottomBase,
    double topBase,
    double height)
    : leftBottom(leftBottom),
    bottomBase(bottomBase),
    topBase(topBase),
    height(height)
{
}


double Trapezoid::getArea() const
{
    return (bottomBase + topBase) * height / 2.0;
}


Point Trapezoid::getCenter() const
{
    return {
        leftBottom.x + bottomBase / 2.0,
        leftBottom.y + height / 2.0
    };
}


void Trapezoid::move(double dx, double dy)
{
    leftBottom.x += dx;
    leftBottom.y += dy;
}


void Trapezoid::scale(double factor)
{
    Point center = getCenter();

    bottomBase *= factor;
    topBase *= factor;
    height *= factor;

    leftBottom.x = center.x - bottomBase / 2.0;
    leftBottom.y = center.y - height / 2.0;
}


const char* Trapezoid::getName() const
{
    return "TRAPEZOID";
}


double Trapezoid::getMinX() const
{
    double topLeftX =
        leftBottom.x + (bottomBase - topBase) / 2.0;

    return std::min(leftBottom.x, topLeftX);
}


double Trapezoid::getMaxX() const
{
    double topLeftX =
        leftBottom.x + (bottomBase - topBase) / 2.0;

    double topRightX = topLeftX + topBase;
    double bottomRightX = leftBottom.x + bottomBase;

    return std::max(bottomRightX, topRightX);
}


double Trapezoid::getMinY() const
{
    return leftBottom.y;
}


double Trapezoid::getMaxY() const
{
    return leftBottom.y + height;
}