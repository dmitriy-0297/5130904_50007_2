#include "rhombus.h"
#include <cmath>
#include <string>
#include "point.h"
#include <iostream>
double Rhombus::getArea() const {
    return std::abs(high_ * weight_) / 2.0;
}
Point Rhombus::getCenter() const {
    return cross_;
}
void Rhombus::move(double x, double y) {
    cross_.x_ += x;
    cross_.y_ += y;
}
std::string Rhombus::getName() const {
    return "RHOMBUS";
}
Rhombus::Rhombus(Point cross, double high, double weight)
    :cross_(cross), high_(high), weight_(weight) {
}
Point Rhombus::getTop() const {
    return Point(cross_.x_, cross_.y_ + high_ / 2);
}

Point Rhombus::getBottom() const {
    return Point(cross_.x_, cross_.y_ - high_ / 2);
}

Point Rhombus::getLeft() const {
    return Point(cross_.x_ - weight_ / 2, cross_.y_);
}

Point Rhombus::getRight() const {
    return Point(cross_.x_ + weight_ / 2, cross_.y_);
}

void Rhombus::scale(double k) {
    if (k <= 0) {
        return;
    }
    high_ *= k;
    weight_ *= k;
}

void Rhombus::print() const {
    std::cout<<getName() << ", ";
    std::cout <<"(" <<getCenter().x_ << ", " <<getCenter().y_ << "), ";
    std::cout<<getArea()<<"\n";
}

