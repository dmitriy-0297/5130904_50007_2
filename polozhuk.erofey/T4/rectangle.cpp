#include "rectangle.h"
#include "shape.h"
#include "point.h"
#include <cmath>
#include <iomanip>
#include <iostream>
#include <string>
double Rectangle::getArea() const {
    double width = std::abs(rh_.x_ - ld_.x_);
    double high = std::abs(rh_.y_ - ld_.y_);
    return width * high;
}
Point Rectangle::getCenter() const {
    std::cout << std::fixed << std::setprecision(2);
    double midX = (ld_.x_ + rh_.x_) / 2.0;
    double targetY = (ld_.y_ + rh_.y_) / 2.0;
    Point center(midX, ld_.y_);
    double step = 0.001;
    while (std::abs(center.y_ - targetY) > step / 2.0) {
        if (center.y_ < targetY) {
            center.y_ += step;
        } else {
            center.y_ -= step;
        }
    }
    return center;
}
void Rectangle::move(double x, double y) {
    ld_.x_ += x;
    ld_.y_ += y;
    rh_.x_ += x;
    rh_.y_ += y;
}
std::string Rectangle::getName() const {
    return "RECTANGLE";
}
void Rectangle::scale(double k) {
    Point center = getCenter();
    ld_.x_ = center.x_ + (ld_.x_ - center.x_) * k;
    ld_.y_ = center.y_ + (ld_.y_ - center.y_) * k;
    rh_.x_ = center.x_ + (rh_.x_ - center.x_) * k;
    rh_.y_ = center.y_ + (rh_.y_ - center.y_) * k;
}
void Rectangle::print() const {
    std::cout << getName();
    std::cout << ", ";
    std::cout <<"(" <<ld_.x_ << ", ";
    std::cout <<ld_.y_ << "), ";
    std::cout<<getArea()<<"\n";
}

