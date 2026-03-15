#include "compositeshape.h"
#include <iomanip>
#include <stdexcept>
#include "shape.h"
#include <memory>
#include "point.h"
#include <string>
#include <iostream>
void CompositeShape::addShape(std::unique_ptr<Shape> shape) {
    shapes_.push_back(std::move(shape));
}
double CompositeShape::getArea() const {
    double bigSum = 0.0;
    for (size_t i = 0; i < shapes_.size(); ++i) {
        bigSum += shapes_[i]->getArea();
    }
    return bigSum;
}
void CompositeShape::move(double x, double y) {
    for (size_t i = 0; i <shapes_.size(); ++i) {
        shapes_[i]->move(x, y);
    }
}
std::string CompositeShape::getName() const {
    return "CompositeShape";
}
Point CompositeShape::getCenter() const {
    if (shapes_.empty()) {
        return Point(0.0, 0.0);
    }
    Point first = shapes_[0]->getCenter();
    double leftX = first.x_;
    double rightX = first.x_;
    double botY = first.y_;
    double topY = first.y_;
    for (size_t i = 1; i < shapes_.size(); ++i) {
        Point now = shapes_[i]->getCenter();
        if (now.x_ < leftX)  leftX = now.x_;
        if (now.x_ > rightX) rightX = now.x_;
        if (now.y_ < botY)   botY = now.y_;
        if (now.y_ > topY)   topY = now.y_;
    }
    double compositeShapeX = (leftX + rightX) / 2.0;
    double compositeShapeY = (botY + topY) / 2.0;
    return Point(compositeShapeX, compositeShapeY);
}
void CompositeShape::scale(double k) {
    if (k > 0 && shapes_.size() > 0) {
        Point center = getCenter();
        for (size_t i = 0; i < shapes_.size(); ++i) {
            Point shapeCenter = shapes_[i]->getCenter();
            double shapeX = shapeCenter.x_ - center.x_;
            double shapeY = shapeCenter.y_ - center.y_;
            shapes_[i]->move(-shapeX, -shapeY);
            shapes_[i]->scale(k);
            shapes_[i]->move(shapeX * k, shapeY * k);
        }
    }
    else {
        throw std::invalid_argument(" k <= 0 or collection is empty");
    }
}
void CompositeShape::print() const {
    std::cout << std::fixed << std::setprecision(2);
    std::cout << "[";
    for (size_t i = 0; i < shapes_.size(); ++i) {
        Point center = shapes_[i]->getCenter();
        std::cout << shapes_[i]->getName()
                  << ", (" << center.x_ << ", " << center.y_ << "), "
                  << shapes_[i]->getArea();
        if (i < shapes_.size() - 1) {
            std::cout << ", ";
        }
    }
    std::cout << "]\n";
}



