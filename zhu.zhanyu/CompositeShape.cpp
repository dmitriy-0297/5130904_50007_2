#include "CompositeShape.h"
#include <algorithm>
#include <new>

void CompositeShape::addShape(std::unique_ptr<Shape> shape) {
    if (!shape) {
        return;
    }
    try {
        shapes_.push_back(std::move(shape));
    }
    catch (const std::bad_alloc& e) {
        throw std::bad_alloc();
    }
}

double CompositeShape::getArea() const {
    double total = 0.0;
    for (const auto& shape : shapes_) {
        total += shape->getArea();
    }
    return total;
}

Point CompositeShape::getCenter() const {
    if (shapes_.empty()) {
        return { 0, 0 };
    }
    double min_x = shapes_[0]->getMinX();
    double max_x = shapes_[0]->getMaxX();
    double min_y = shapes_[0]->getMinY();
    double max_y = shapes_[0]->getMaxY();
    for (const auto& shape : shapes_) {
        min_x = std::min(min_x, shape->getMinX());
        max_x = std::max(max_x, shape->getMaxX());
        min_y = std::min(min_y, shape->getMinY());
        max_y = std::max(max_y, shape->getMaxY());
    }
    return { (min_x + max_x) / 2, (min_y + max_y) / 2 };
}

void CompositeShape::move(double dx, double dy) {
    for (auto& shape : shapes_) {
        shape->move(dx, dy);
    }
}

void CompositeShape::scale(double factor) {
    Point compositeCenter = getCenter();
    for (auto& shape : shapes_) {
        Point shapeCenter = shape->getCenter();
        double dx = shapeCenter.x - compositeCenter.x;
        double dy = shapeCenter.y - compositeCenter.y;
        double newDx = dx * factor;
        double newDy = dy * factor;
        Point newShapeCenter;
        newShapeCenter.x = compositeCenter.x + newDx;
        newShapeCenter.y = compositeCenter.y + newDy;
        double moveX = newShapeCenter.x - shapeCenter.x;
        double moveY = newShapeCenter.y - shapeCenter.y;
        shape->move(moveX, moveY);
        shape->scale(factor);
    }
}

std::string CompositeShape::getName() const {
    return "COMPOSITE";
}

double CompositeShape::getMinX() const {
    if (shapes_.empty()) return 0;
    double min_x = shapes_[0]->getMinX();
    for (const auto& shape : shapes_) {
        min_x = std::min(min_x, shape->getMinX());
    }
    return min_x;
}

double CompositeShape::getMinY() const {
    if (shapes_.empty()) return 0;
    double min_y = shapes_[0]->getMinY();
    for (const auto& shape : shapes_) {
        min_y = std::min(min_y, shape->getMinY());
    }
    return min_y;
}

double CompositeShape::getMaxX() const {
    if (shapes_.empty()) return 0;
    double max_x = shapes_[0]->getMaxX();
    for (const auto& shape : shapes_) {
        max_x = std::max(max_x, shape->getMaxX());
    }
    return max_x;
}

double CompositeShape::getMaxY() const {
    if (shapes_.empty()) return 0;
    double max_y = shapes_[0]->getMaxY();
    for (const auto& shape : shapes_) {
        max_y = std::max(max_y, shape->getMaxY());
    }
    return max_y;
}
