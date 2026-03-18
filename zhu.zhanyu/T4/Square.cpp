#include"Square.h"
#include <stdexcept>
Square::Square(Point leftBottom, double size) :leftBottom_(leftBottom), size_(size) {
    if (size <= 0) {
        throw std::invalid_argument("Square side length must be positive");
    }
}
double Square::getArea()const {
    return size_ * size_;
}
Point Square::getCenter()const {
    Point Center;
    Center.x = leftBottom_.x + size_ / 2;
    Center.y = leftBottom_.y ;
}
void Square::move(double dx, double dy) {
    leftBottom_.x += dx;
    leftBottom_.y += dy;
}
void Square::scale(double factor) {
    if (factor <= 0) {
        throw std::invalid_argument("Scale factor must be positive");
    }
    Point Center = getCenter();
    size_ *= factor;
    leftBottom_.x = Center.x - size_ / 2;
    leftBottom_.y = Center.y - size_ / 2;
}
std::string Square::getName()const {
    return "SQUARE";
}
double Square::getMinX() const { return leftBottom_.x; }
double Square::getMaxX() const { return leftBottom_.x + size_; }
double Square::getMinY() const { return leftBottom_.y; }
double Square::getMaxY() const { return leftBottom_.y + size_; }

