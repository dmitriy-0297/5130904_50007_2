#include "compositeshape.h"
#include <iostream>
#include <iomanip>
#include <algorithm>

CompositeShape::CompositeShape() {}

void CompositeShape::addShape(std::unique_ptr<Shape> shape) {
    shapes.push_back(std::move(shape));
}

Point CompositeShape::getBoundingBoxMin() const {
    if (shapes.empty()) {
        return Point(0.0, 0.0);
    }

    double minX = shapes[0]->getCenter().x;
    double minY = shapes[0]->getCenter().y;

    for (const auto& shape : shapes) {
        Point center = shape->getCenter();
        if (center.x < minX) minX = center.x;
        if (center.y < minY) minY = center.y;
    }

    return Point(minX, minY);
}

Point CompositeShape::getBoundingBoxMax() const {
    if (shapes.empty()) {
        return Point(0.0, 0.0);
    }

    double maxX = shapes[0]->getCenter().x;
    double maxY = shapes[0]->getCenter().y;

    for (const auto& shape : shapes) {
        Point center = shape->getCenter();
        if (center.x > maxX) maxX = center.x;
        if (center.y > maxY) maxY = center.y;
    }

    return Point(maxX, maxY);
}

double CompositeShape::getArea() const {
    double totalArea = 0.0;
    for (const auto& shape : shapes) {
        totalArea += shape->getArea();
    }
    return totalArea;
}

Point CompositeShape::getCenter() const {
    Point min = getBoundingBoxMin();
    Point max = getBoundingBoxMax();
    return Point((min.x + max.x) / 2.0, (min.y + max.y) / 2.0);
}

void CompositeShape::move(double dx, double dy) {
    for (auto& shape : shapes) {
        shape->move(dx, dy);
    }
}

void CompositeShape::scale(double coefficient) {
    Point center = getCenter();

    for (auto& shape : shapes) {
        Point shapeCenter = shape->getCenter();

        double dx = (shapeCenter.x - center.x) * coefficient;
        double dy = (shapeCenter.y - center.y) * coefficient;

        shape->move(center.x - shapeCenter.x, center.y - shapeCenter.y);
        shape->scale(coefficient);
        shape->move(dx, dy);
    }
}

const char* CompositeShape::getName() const {
    return "COMPOSITE";
}

void CompositeShape::print() const {
    std::cout << std::fixed << std::setprecision(2);
    std::cout << "[" << getName() << ", (" << getCenter().x << ", "
              << getCenter().y << "), " << getArea() << ":" << std::endl;

    for (size_t i = 0; i < shapes.size(); i++) {
        std::cout << "  ";
        shapes[i]->print();
        if (i < shapes.size() - 1) {
            std::cout << ",";
        }
        std::cout << std::endl;
    }

    std::cout << "]";
}