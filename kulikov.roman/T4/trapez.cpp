#include "trapez.h"
#include <iostream>
#include <iomanip>

Trapezoid::Trapezoid(Point bl, double bb, double tb, double h)
    : Left(bl), Base(bb), topBase(tb), height(h) {}

double Trapezoid::getArea() const {
    return (Base + topBase) * height / 2.0;
}

Point Trapezoid::getCenter() const {
    double centerX = Left.x + (Base + topBase) / 4.0;
    double centerY = Left.y + height / 2.0;
    return Point(centerX, centerY);
}

void Trapezoid::move(double dx, double dy) {
    Left.x += dx;
    Left.y += dy;
}

void Trapezoid::scale(double coefficient) {
    Point center = getCenter();

    Base *= coefficient;
    topBase *= coefficient;
    height *= coefficient;

    Left.x = center.x - (Base + topBase) / 4.0;
    Left.y = center.y - height / 2.0;
}

const char* Trapezoid::getName() const {
    return "TRAPEZOID";
}

void Trapezoid::print() const {
    std::cout << std::fixed << std::setprecision(2);
    std::cout << "[" << getName() << ", (" << getCenter().x << ", "
              << getCenter().y << "), " << getArea() << "]";
}
